#include "mpu6500.h"
#include "stm32f4xx_hal_flash_ex.h"
#include "stm32f4xx_hal_flash.h"


#define BOARD_DOWN (0)

#define MPU_HSPI hspi4
#define MPU_NSS_LOW HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET)
#define MPU_NSS_HIGH HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET)

#define Kp 2.0f
#define Ki 0.01f
volatile float q0 = 1.0f;
volatile float q1 = 0.0f;
volatile float q2 = 0.0f;
volatile float q3 = 0.0f;
volatile float exInt, eyInt, ezInt;
static volatile float gx, gy, gz, ax, ay, az, mx, my, mz;
volatile uint32_t last_update, now_update;
static uint8_t tx, rx;
static uint8_t tx_buff[14] = {0xff};
uint8_t mpu_buff[14]={0}; 
mpu_data_t mpu_data={0};

imu_t imu = {
	.Tar_Temp = IMU_TAR_TEMP
};



//计算平方根
float inv_sqrt(float x)
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long *)&y;

	i = 0x5f3759df - (i >> 1);
	y = *(float *)&i;
	y = y * (1.5f - (halfx * y * y));

	return y;
}


//向reg处写一个字节的数据
uint8_t mpu_write_byte(uint8_t const reg, uint8_t const data)
{
	MPU_NSS_LOW;
	tx = reg & 0x7F;
	HAL_SPI_TransmitReceive(&MPU_HSPI, &tx, &rx, 1, 55);
	tx = data;
	HAL_SPI_TransmitReceive(&MPU_HSPI, &tx, &rx, 1, 55);
	MPU_NSS_HIGH;
	return 0;
}


//读取reg处的数据
uint8_t mpu_read_byte(uint8_t const reg)
{
	MPU_NSS_LOW;
	tx = reg | 0x80;
	HAL_SPI_TransmitReceive(&MPU_HSPI, &tx, &rx, 1, 55);
	HAL_SPI_TransmitReceive(&MPU_HSPI, &tx, &rx, 1, 55);
	MPU_NSS_HIGH;
	return rx;
}

//从regAddr开始通过SPI读取len个字节的数据
uint8_t mpu_read_bytes(uint8_t const regAddr, uint8_t *pData, uint8_t len)
{
	MPU_NSS_LOW;
	tx = regAddr | 0x80;
	tx_buff[0] = tx;
	HAL_SPI_TransmitReceive(&MPU_HSPI, &tx, &rx, 1, 55);
	HAL_SPI_TransmitReceive(&MPU_HSPI, tx_buff, pData, len, 55);
	MPU_NSS_HIGH;
	return 0;
}



//获取陀螺仪数据
void mpu_get_data(void)
{
	mpu_read_bytes(MPU6500_ACCEL_XOUT_H, mpu_buff, 14);   //读取寄存器存储数据
	

	//加速度数据
	mpu_data.ax = mpu_buff[0] << 8 | mpu_buff[1];         //加速度
	mpu_data.ay = mpu_buff[2] << 8 | mpu_buff[3];         //加速度
	mpu_data.az = mpu_buff[4] << 8 | mpu_buff[5];         //加速度
	
	//温度
	mpu_data.temp = mpu_buff[6] << 8 | mpu_buff[7];
  
	//陀螺仪数据	
	mpu_data.gx = ((mpu_buff[8] << 8 | mpu_buff[9]) - mpu_data.gx_offset);       //角速度
	mpu_data.gy = ((mpu_buff[10] << 8 | mpu_buff[11]) - mpu_data.gy_offset);     //角速度
	mpu_data.gz = ((mpu_buff[12] << 8 | mpu_buff[13]) - mpu_data.gz_offset);     //角速度
  
	memcpy(&imu.ax, &mpu_data.ax, 6 * sizeof(int16_t));
  
	if(abs(mpu_data.gx)<5)mpu_data.gx=0;
	if(abs(mpu_data.gy)<5)mpu_data.gy=0;
	if(abs(mpu_data.gz)<5)mpu_data.gz=0;
	//温度转换
	imu.temp = 21 + mpu_data.temp / 333.87f;
	
	/* 2000dps -> rad/s */
	imu.wx = mpu_data.gx / 16.384f / 57.3f;  //rol         
	imu.wy = mpu_data.gy / 16.384f / 57.3f;  //pitch       
	imu.wz = mpu_data.gz / 16.384f / 57.3f;  //yaw  
	//printf("yaw=%d,pit=%d,rol=%d\r\n",mpu_data.gx,mpu_data.gy,mpu_data.gz);
}



uint8_t mpu_set_gyro_fsr(uint8_t fsr)
{
	return mpu_write_byte(MPU6500_GYRO_CONFIG, fsr << 3);
}


uint8_t mpu_set_accel_fsr(uint8_t fsr)
{
	return mpu_write_byte(MPU6500_ACCEL_CONFIG, fsr << 3);
}


//零飘计算
void mpu_offset_call(void)
{
	int i;
	uint32_t WriteFlashData[3];//存入数据
	uint32_t WriteFlashADDR = 0x0810C000;
	for (i = 0; i < 1000; i++){
		mpu_read_bytes(MPU6500_ACCEL_XOUT_H, mpu_buff, 14);   //读取寄存器存储数据

	//加速度数据
	mpu_data.ax = mpu_buff[0] << 8 | mpu_buff[1];         //加速度
	mpu_data.ay = mpu_buff[2] << 8 | mpu_buff[3];         //加速度
	mpu_data.az = mpu_buff[4] << 8 | mpu_buff[5];         //加速度
	
	//温度
	mpu_data.temp = mpu_buff[6] << 8 | mpu_buff[7];
  
	//陀螺仪数据	
	mpu_data.gx = ((mpu_buff[8] << 8 | mpu_buff[9]) - mpu_data.gx_offset);       //角速度
	mpu_data.gy = ((mpu_buff[10] << 8 | mpu_buff[11]) - mpu_data.gy_offset);     //角速度
	mpu_data.gz = ((mpu_buff[12] << 8 | mpu_buff[13]) - mpu_data.gz_offset);     //角速度

	memcpy(&imu.ax, &mpu_data.ax, 6 * sizeof(int16_t));

	//温度转换
	imu.temp = 21 + mpu_data.temp / 333.87f;	

	}
	HAL_Delay(1000);
	for (i = 0; i < 200; i++)
	{
		mpu_read_bytes(MPU6500_ACCEL_XOUT_H, mpu_buff, 14);
		mpu_data.ax_offset += mpu_buff[0] << 8 | mpu_buff[1];
		mpu_data.ay_offset += mpu_buff[2] << 8 | mpu_buff[3];
		mpu_data.az_offset += mpu_buff[4] << 8 | mpu_buff[5];

		mpu_data.gx_offset += mpu_buff[8] << 8 | mpu_buff[9];
		mpu_data.gy_offset += mpu_buff[10] << 8 | mpu_buff[11];
		mpu_data.gz_offset += mpu_buff[12] << 8 | mpu_buff[13];
		
		HAL_Delay(1);
	}
	
	mpu_data.ax_offset = mpu_data.ax_offset / 200;
	mpu_data.ay_offset = mpu_data.ay_offset / 200;
	mpu_data.az_offset = mpu_data.az_offset / 200;
	mpu_data.gx_offset = mpu_data.gx_offset / 200;
	mpu_data.gy_offset = mpu_data.gy_offset / 200;
	mpu_data.gz_offset = mpu_data.gz_offset / 200;
	
	
	//printf("%d,%d,%d,%d,%d,%d,\r\n",mpu_data.ax_offset,mpu_data.ay_offset,mpu_data.az_offset,mpu_data.gx_offset,mpu_data.gy_offset,mpu_data.gz_offset);
	
	memcpy(&WriteFlashData, &mpu_data.ax_offset, 2);
	memcpy((void *)((uint32_t)(&WriteFlashData)+2), &mpu_data.ay_offset, 2);
	memcpy((void *)((uint32_t)(&WriteFlashData)+4), &mpu_data.az_offset, 2);
    memcpy((void *)((uint32_t)(&WriteFlashData)+6), &mpu_data.gx_offset, 2);
	memcpy((void *)((uint32_t)(&WriteFlashData)+8), &mpu_data.gy_offset, 2);
    memcpy((void *)((uint32_t)(&WriteFlashData)+10), &mpu_data.gz_offset, 2);
	
	memcpy(&mpu_data.ax_offset, (void *)(WriteFlashADDR), 2);
	memcpy(&mpu_data.ay_offset, (void *)(WriteFlashADDR+2), 2);
	memcpy(&mpu_data.az_offset, (void *)(WriteFlashADDR+4), 2);
	memcpy(&mpu_data.gx_offset, (void *)(WriteFlashADDR+6), 2);
	memcpy(&mpu_data.gy_offset, (void *)(WriteFlashADDR+8), 2);
    memcpy(&mpu_data.gz_offset, (void *)(WriteFlashADDR+10), 2);
	printf("%d,%d,%d,%d,%d,%d,\r\n",mpu_data.ax_offset,mpu_data.ay_offset,mpu_data.az_offset,mpu_data.gx_offset,mpu_data.gy_offset,mpu_data.gz_offset);

	WriteFlash(3,WriteFlashData,WriteFlashADDR);

	
	for(int i=0;i<3;i++)
	{
		printf("\naddr is:0x%x, data is:0x%x\r\n", WriteFlashADDR+i*4,  *(__IO uint32_t*)(WriteFlashADDR+i*4));
	}
	
	memcpy(&mpu_data.ax_offset, (void *)(WriteFlashADDR), 2);
	memcpy(&mpu_data.ay_offset, (void *)(WriteFlashADDR+2), 2);
	memcpy(&mpu_data.az_offset, (void *)(WriteFlashADDR+4), 2);
	memcpy(&mpu_data.gx_offset, (void *)(WriteFlashADDR+6), 2);
	memcpy(&mpu_data.gy_offset, (void *)(WriteFlashADDR+8), 2);
    memcpy(&mpu_data.gz_offset, (void *)(WriteFlashADDR+10), 2);

	printf("%d,%d,%d,%d,%d,%d,\r\n",mpu_data.ax_offset,mpu_data.ay_offset,mpu_data.az_offset,mpu_data.gx_offset,mpu_data.gy_offset,mpu_data.gz_offset);
}


uint8_t id;

//MPU初始化
uint8_t mpu_device_init(void)
{
	  uint8_t MPU6500_Init_Data[10][2] = {
		{MPU6500_PWR_MGMT_1, 0x80},		/* Reset Device */
		{MPU6500_PWR_MGMT_1, 0x03},		/* Clock Source - Gyro-Z */
		{MPU6500_PWR_MGMT_2, 0x00},		/* Enable Acc & Gyro */
		{MPU6500_CONFIG, 0x04},			/* LPF 41Hz */
		{MPU6500_GYRO_CONFIG, 0x18},	/* +-2000dps */
		{MPU6500_ACCEL_CONFIG, 0x10},   /* +-8G */
		{MPU6500_ACCEL_CONFIG_2, 0x02}, /* enable LowPassFilter  Set Acc LPF */
		{MPU6500_USER_CTRL, 0x20},
	}; /* Enable AUX */
	id = mpu_read_byte(MPU6500_WHO_AM_I);            //读取陀螺仪地址
	//printf("n%x\r\n",id);
	uint8_t i = 0;
	for (i = 0; i < 10; i++)
	{
		mpu_write_byte(MPU6500_Init_Data[i][0], MPU6500_Init_Data[i][1]);
		HAL_Delay(10);
	}
	HAL_Delay(10);
	mpu_offset_call();    //计算零漂
  
	return 0;
}





//四元数初始化
void init_quaternion(void)
{
	int16_t hx, hy; //hz;

	hx = imu.mx;
	hy = imu.my;
	//hz = imu.mz;

	
#ifdef BOARD_DOWN
	if (hx < 0 && hy < 0)
	{
		if (fabs(hx / hy) >= 1)
		{
			q0 = -0.005;
			q1 = -0.199;
			q2 = 0.979;
			q3 = -0.0089;
		}
		else
		{
			q0 = -0.008;
			q1 = -0.555;
			q2 = 0.83;
			q3 = -0.002;
		}
	}
	else if (hx < 0 && hy > 0)
	{
		if (fabs(hx / hy) >= 1)
		{
			q0 = 0.005;
			q1 = -0.199;
			q2 = -0.978;
			q3 = 0.012;
		}
		else
		{
			q0 = 0.005;
			q1 = -0.553;
			q2 = -0.83;
			q3 = -0.0023;
		}
	}
	else if (hx > 0 && hy > 0)
	{
		if (fabs(hx / hy) >= 1)
		{
			q0 = 0.0012;
			q1 = -0.978;
			q2 = -0.199;
			q3 = -0.005;
		}
		else
		{
			q0 = 0.0023;
			q1 = -0.83;
			q2 = -0.553;
			q3 = 0.0023;
		}
	}
	else if (hx > 0 && hy < 0)
	{
		if (fabs(hx / hy) >= 1)
		{
			q0 = 0.0025;
			q1 = 0.978;
			q2 = -0.199;
			q3 = 0.008;
		}
		else
		{
			q0 = 0.0025;
			q1 = 0.83;
			q2 = -0.56;
			q3 = 0.0045;
		}
	}
#else
	if (hx < 0 && hy < 0)
	{
		if (fabs(hx / hy) >= 1)
		{
			q0 = 0.195;
			q1 = -0.015;
			q2 = 0.0043;
			q3 = 0.979;
		}
		else
		{
			q0 = 0.555;
			q1 = -0.015;
			q2 = 0.006;
			q3 = 0.829;
		}
	}
	else if (hx < 0 && hy > 0)
	{
		if (fabs(hx / hy) >= 1)
		{
			q0 = -0.193;
			q1 = -0.009;
			q2 = -0.006;
			q3 = 0.979;
		}
		else
		{
			q0 = -0.552;
			q1 = -0.0048;
			q2 = -0.0115;
			q3 = 0.8313;
		}
	}
	else if (hx > 0 && hy > 0)
	{
		if (fabs(hx / hy) >= 1)
		{
			q0 = -0.9785;
			q1 = 0.008;
			q2 = -0.02;
			q3 = 0.195;
		}
		else
		{
			q0 = -0.9828;
			q1 = 0.002;
			q2 = -0.0167;
			q3 = 0.5557;
		}
	}
	else if (hx > 0 && hy < 0)
	{
		if (fabs(hx / hy) >= 1)
		{
			q0 = -0.979;
			q1 = 0.0116;
			q2 = -0.0167;
			q3 = -0.195;
		}
		else
		{
			q0 = -0.83;
			q1 = 0.014;
			q2 = -0.012;
			q3 = -0.556;
		}
	}
#endif
}


//四元数解算
void imu_ahrs_update(void)
{
	float norm;
	float hx, hy, hz, bx, bz;
	float vx, vy, vz, wx, wy, wz;
	float ex, ey, ez, halfT;
	float tempq0, tempq1, tempq2, tempq3;

	float q0q0 = q0 * q0;
	float q0q1 = q0 * q1;
	float q0q2 = q0 * q2;
	float q0q3 = q0 * q3;
	float q1q1 = q1 * q1;
	float q1q2 = q1 * q2;
	float q1q3 = q1 * q3;
	float q2q2 = q2 * q2;
	float q2q3 = q2 * q3;
	float q3q3 = q3 * q3;

	gx = imu.wx;
	gy = imu.wy;
	gz = imu.wz;
	ax = imu.ax;
	ay = imu.ay;
	az = imu.az;
	mx = imu.mx;
	my = imu.my;
	mz = imu.mz;

	now_update = HAL_GetTick(); //ms
	halfT = ((float)(now_update - last_update) / 2000.0f);

	last_update = now_update;


	/* Fast inverse square-root */
	norm = inv_sqrt(ax * ax + ay * ay + az * az);
	ax = ax * norm;
	ay = ay * norm;
	az = az * norm;

#ifdef IST8310
	norm = inv_sqrt(mx * mx + my * my + mz * mz);   //????????
	mx = mx * norm;
	my = my * norm;
	mz = mz * norm;
#else
	mx = 0;
	my = 0;
	mz = 0;
#endif
	/* compute reference direction of flux */
	hx = 2.0f * mx * (0.5f - q2q2 - q3q3) + 2.0f * my * (q1q2 - q0q3) + 2.0f * mz * (q1q3 + q0q2);
	hy = 2.0f * mx * (q1q2 + q0q3) + 2.0f * my * (0.5f - q1q1 - q3q3) + 2.0f * mz * (q2q3 - q0q1);
	hz = 2.0f * mx * (q1q3 - q0q2) + 2.0f * my * (q2q3 + q0q1) + 2.0f * mz * (0.5f - q1q1 - q2q2);
	bx = sqrt((hx * hx) + (hy * hy));
	bz = hz;

	/* estimated direction of gravity and flux (v and w) */
	vx = 2.0f * (q1q3 - q0q2);
	vy = 2.0f * (q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3;
	
	
	wx = 2.0f * bx * (0.5f - q2q2 - q3q3) + 2.0f * bz * (q1q3 - q0q2);
	wy = 2.0f * bx * (q1q2 - q0q3) + 2.0f * bz * (q0q1 + q2q3);
	wz = 2.0f * bx * (q0q2 + q1q3) + 2.0f * bz * (0.5f - q1q1 - q2q2);

	/* 
	 * error is sum of cross product between reference direction 
	 * of fields and direction measured by sensors 
	 */
	ex = (ay * vz - az * vy) + (my * wz - mz * wy);
	ey = (az * vx - ax * vz) + (mz * wx - mx * wz);
	ez = (ax * vy - ay * vx) + (mx * wy - my * wx);

	/* PI */
	if (ex != 0.0f && ey != 0.0f && ez != 0.0f)   //??????
	{
		exInt = exInt + ex * Ki * halfT;            //??????Ki
		eyInt = eyInt + ey * Ki * halfT;
		ezInt = ezInt + ez * Ki * halfT;

		gx = gx + Kp * ex + exInt;
		gy = gy + Kp * ey + eyInt;
		gz = gz + Kp * ez + ezInt;
	}


	tempq0 = q0 + (-q1 * gx - q2 * gy - q3 * gz) * halfT;   
	tempq1 = q1 + (q0 * gx + q2 * gz - q3 * gy) * halfT;
	tempq2 = q2 + (q0 * gy - q1 * gz + q3 * gx) * halfT;
	tempq3 = q3 + (q0 * gz + q1 * gy - q2 * gx) * halfT;

	/* normalise quaternion */
	norm = inv_sqrt(tempq0 * tempq0 + tempq1 * tempq1 + tempq2 * tempq2 + tempq3 * tempq3);
	q0 = tempq0 * norm;	
	q1 = tempq1 * norm;
	q2 = tempq2 * norm;
	q3 = tempq3 * norm;
}

//姿态角更新
void imu_attitude_update(void)
{
	/* yaw    -pi----pi */
	imu.yaw = -atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1) * 57.3;
	/* pitch  -pi/2----pi/2 */
	imu.pit = -asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3;
	/* roll   -pi----pi  */
	imu.rol = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3;

  //printf("yaw=%f,pit=%f,rol=%f\r\n",imu.yaw,imu.pit,imu.rol);
}



static void WriteFlash(uint32_t L,uint32_t Data[],uint32_t addr)
{
	uint32_t i=0;

	HAL_FLASH_Unlock();

	FLASH_EraseInitTypeDef EraseInitStruct;
	
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector = FLASH_SECTOR_15; //Specify sector number
	EraseInitStruct.NbSectors = 1; //This is also important!
	uint32_t SectorError = 0;
	if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK) {
    //Erase error!
}

	for(i=0;i<L;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr+4*i, Data[i]);
	}

	HAL_FLASH_Lock();
}




