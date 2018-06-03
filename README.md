# Self-Balancing-PID-controlled-Robot
Following Steps are implemented in this algorithm :
--> Gyrometer must be calibrated on start up every single time to get consistent readings
--> MPU-6050 is used as a gyrometer to monitor Yaw angle readings 
--> DMP must not be initialised before calibration is complete. 
--> So in the Void loop first finish calibration that is setting offsets and then do dmp.initialise()
--> error is calculated and fed to the PID equation which controls the motors.

