#ifndef __MX_64_2_0__
#define __MX_64_2_0__

// Address	Size(Byte)	Data Name	Description	Access	Initial
// Value
// 0	2	Model Number	Model Number	R	311
// 2	4	Model Information	Model Information	R	-
// 6	1	Firmware Version	Firmware Version	R	-
// 7	1	ID	DYNAMIXEL ID	RW	1
// 8	1	Baud Rate	Communication Baud Rate	RW	1
// 9	1	Return Delay Time	Response Delay Time	RW	250
// 10	1	Drive Mode	Drive Mode	RW	0
// 11	1	Operating Mode	Operating Mode	RW	3
// 12	1	Secondary(Shadow) ID	Secondary ID	RW	255
// 13	1	Protocol Type	Protocol Type	RW	2
// 20	4	Homing Offset	Home Position Offset	RW	0
// 24	4	Moving Threshold	Velocity Threshold for Movement Detection	RW	10
// 31	1	Temperature Limit	Maximum Internal Temperature Limit	RW	80
// 32	2	Max Voltage Limit	Maximum Input Voltage Limit	RW	160
// 34	2	Min Voltage Limit	Minimum Input Voltage Limit	RW	95
// 36	2	PWM Limit	Maximum PWM Limit	RW	885
// 38	2	Current Limit	Maximum Current Limit	RW	1941
// 40	4	Acceleration Limit	Maximum Acceleration Limit	RW	32767
// 44	4	Velocity Limit	Maximum Velocity Limit	RW	285
// 48	4	Max Position Limit	Maximum Position Limit	RW	4,095
// 52	4	Min Position Limit	Minimum Position Limit	RW	0
// 63	1	Shutdown	Shutdown Error Information	RW	52
namespace MX64_2_0_EEPROM {
    const uint8_t MODEL_NUMBER = 0;
    const uint8_t MODEL_INFORMATION = 2;
    const uint8_t FIRMWARE_VERSION = 6;
    const uint8_t ID = 7;
    const uint8_t BAUD_RATE = 8;
    const uint8_t RETURN_DELAY = 9;
    const uint8_t DRIVE_MODE = 10;
    const uint8_t OPERATING_MODE = 11;
    const uint8_t SECONDARY_ID = 12;
    const uint8_t PROTOCOL_TYPE = 13;
    const uint8_t HOMING_OFFSET = 20;
    const uint8_t MOVING_THRESHOLD = 24;
    const uint8_t TEMPERATURE_LIMIT = 31;
    const uint8_t MAX_VOLTAGE_LIMIT = 32;
    const uint8_t MIN_VOLTAGE_LIMIT = 34;
    const uint8_t PWM_LIMIT = 36;
    const uint8_t CURRENT_LIMIT = 38;
    const uint8_t ACCELERATION_LIMIT = 40;
    const uint8_t VELOCITY_LIMIT = 44;
    const uint8_t MAX_POSITION_LIMIT = 48;
    const uint8_t MIN_POSITION_LIMIT = 52;
    const uint8_t SHUTDOWN = 63;
};

// Address	Size(Byte)	Data Name	Description	Access	Initial
// Value
// 64	1	Torque Enable	Motor Torque On/Off	RW	0
// 65	1	LED	Status LED On/Off	RW	0
// 68	1	Status Return Level	Select Types of Status Return	RW	2
// 69	1	Registered Instruction	REG_WRITE Instruction Flag	R	0
// 70	1	Hardware Error Status	Hardware Error Status	R	0
// 76	2	Velocity I Gain	I Gain of Velocity	RW	1920
// 78	2	Velocity P Gain	P Gain of Velocity	RW	100
// 80	2	Position D Gain	D Gain of Position	RW	0
// 82	2	Position I Gain	I Gain of Position	RW	0
// 84	2	Position P Gain	P Gain of Position	RW	850
// 88	2	Feedforward 2nd Gain	2nd Gain of Feed-Forward	RW	0
// 90	2	Feedforward 1st Gain	1st Gain of Feed-Forward	RW	0
// 98	1	BUS Watchdog	DYNAMIXEL BUS Watchdog	RW	0
// 100	2	Goal PWM	Desired PWM Value	RW	-
// 102	2	Goal Current	Desired Current Value	RW	-
// 104	4	Goal Velocity	Desired Velocity Value	RW	-
// 108	4	Profile Acceleration	Acceleration Value of Profile	RW	0
// 112	4	Profile Velocity	Velocity Value of Profile	RW	0
// 116	4	Goal Position	Desired Position	RW	-
// 120	2	Realtime Tick	Count Time in Millisecond	R	-
// 122	1	Moving	Movement Flag	R	0
// 123	1	Moving Status	Detailed Information of Movement Status	R	0
// 124	2	Present PWM	Present PWM Value	R	-
// 126	2	Present Current	Present Current Value	R	-
// 128	4	Present Velocity	Present Velocity Value	R	-
// 132	4	Present Position	Present Position Value	R	-
// 136	4	Velocity Trajectory	Desired Velocity Trajectory from Profile	R	-
// 140	4	Position Trajectory	Desired Position Trajectory from Profile	R	-
// 144	2	Present Input Voltage	Present Input Voltage	R	-
// 146	1	Present Temperature	Present Internal Temperature	R	-
// 168	2	Indirect Address 1	Indirect Address 1	RW	224
// 170	2	Indirect Address 2	Indirect Address 2	RW	225
// 172	2	Indirect Address 3	Indirect Address 3	RW	226
// …	…	…	…	…	…
// 218	2	Indirect Address 26	Indirect Address 26	RW	249
// 220	2	Indirect Address 27	Indirect Address 27	RW	250
// 222	2	Indirect Address 28	Indirect Address 28	RW	251
// 224	1	Indirect Data 1	Indirect Data 1	RW	0
// 225	1	Indirect Data 2	Indirect Data 2	RW	0
// 226	1	Indirect Data 3	Indirect Data 3	RW	0
// …	…	…	…	…	…
// 249	1	Indirect Data 26	Indirect Data 26	RW	0
// 250	1	Indirect Data 27	Indirect Data 27	RW	0
// 251	1	Indirect Data 28	Indirect Data 28	RW	0
// 578	2	Indirect Address 29	Indirect Address 29	RW	634
// 580	2	Indirect Address 30	Indirect Address 30	RW	635
// 582	2	Indirect Address 31	Indirect Address 31	RW	636
// …	…	…	…	…	…
// 628	2	Indirect Address 54	Indirect Address 54	RW	659
// 630	2	Indirect Address 55	Indirect Address 55	RW	660
// 632	2	Indirect Address 56	Indirect Address 56	RW	661
// 634	1	Indirect Data 29	Indirect Data 29	RW	0
// 635	1	Indirect Data 30	Indirect Data 30	RW	0
// 636	1	Indirect Data 31	Indirect Data 31	RW	0
// …	…	…	…	…	…
// 659	1	Indirect Data 54	Indirect Data 54	RW	0
// 660	1	Indirect Data 55	Indirect Data 55	RW	0
// 661	1	Indirect Data 56	Indirect Data 56	RW	0
namespace MX64_2_0_RAM {
    const uint8_t TORQUE_ENABLE = 64;
    const uint8_t LED = 65;
    const uint8_t STATUS_RETURN_LEVEL = 68;
    const uint8_t REGISTERED_INSTRUCTION = 69;
    const uint8_t HARDWARE_ERROR_STATUS = 70;
    const uint8_t VELOCITY_I_GAIN = 76;
    const uint8_t VELOCITY_P_GAIN = 78;
    const uint8_t POSITION_D_GAIN = 80;
    const uint8_t POSITION_I_GAIN = 82;
    const uint8_t POSITION_P_GAIN = 84;
    const uint8_t FEEDFORWARD_2ND_GAIN = 88;
    const uint8_t FEEDFORWARD_1ST_GAIN = 90;
    const uint8_t BUS_WATCHDOG = 98;
    const uint8_t GOAL_PWM = 100;
    const uint8_t GOAL_CURRENT = 102;
    const uint8_t GOAL_VELOCITY = 104;
    const uint8_t PROFILE_ACCELERATION = 108;
    const uint8_t PROFILE_VELOCITY = 112;
    const uint8_t GOAL_POSITION = 116;
    const uint8_t REALTIME_TICK = 120;
    const uint8_t MOVING = 122;
    const uint8_t MOVING_STATUS = 123;
    const uint8_t PRESENT_PWM = 124;
    const uint8_t PRESENT_CURRENT = 126;
    const uint8_t PRESENT_VELOCITY = 128;
    const uint8_t PRESENT_POSITION = 132;
    const uint8_t VELOCITY_TRAJECTORY = 136;
    const uint8_t POSITION_TRAJECTORY = 140;
    const uint8_t PRESENT_INPUT_VOLTAGE = 144;
    const uint8_t PRESENT_TEMPERATURE = 146;

    const uint8_t INDIRECT_ADDRESS_1 = 168;
    const uint8_t INDIRECT_ADDRESS_2 = 170;
    const uint8_t INDIRECT_ADDRESS_3 = 172;
    const uint8_t INDIRECT_ADDRESS_26 = 218;
    const uint8_t INDIRECT_ADDRESS_27 = 220;
    const uint8_t INDIRECT_ADDRESS_28 = 222;
    const uint8_t INDIRECT_DATA_1 = 224;
    const uint8_t INDIRECT_DATA_2 = 225;
    const uint8_t INDIRECT_DATA_3 = 226;
    const uint8_t INDIRECT_DATA_26 = 249;
    const uint8_t INDIRECT_DATA_27 = 250;
    const uint8_t INDIRECT_DATA_28 = 251;

}; // namespace MX64_RAM


#endif