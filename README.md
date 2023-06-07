# CPP Senior Project

## December 2022 - May 2023
ROS Noetic control GPS autonomous vehicle using ATMega2560 and Jetson Nano. 

## Video Demo
**Autonomous Mode:**        https://youtu.be/Os8QsEWLSAU

**Controller Mode:**        https://youtu.be/IqCc2zWrYO0

**Test Controller Mode:**   https://youtu.be/60IaMCZQ1AA

### Mechanical 
![image](https://github.com/Chrisx19/senior_project_AV/assets/107272321/af685e61-d4f0-4aca-9ad1-e9731f64c4dd)


### Electrical


**PCB Design**
  Reason initial design is included because of the mistakes my team learned while testing. Due to small traces with about 3 Amps going inside of our traces, it blew up and it came out of the PCB. Therefore, we learned from mistakes and calculated how much mil we need for our traces that can withstand about 4 Amps.
  
* **Initial Design: 30 mil traces**

![image](https://github.com/Chrisx19/senior_project_AV/assets/107272321/7f8c054f-1a8e-4ad8-a297-a58b69e2f1f4)


* **Final Design: 150 mil traces**

![schematic](https://github.com/Chrisx19/senior_project_AV/assets/107272321/bf76930d-2566-4377-8d29-5cfc3acadb3e)

![Traces](https://github.com/Chrisx19/senior_project_AV/assets/107272321/b528a5ff-9bfa-4a72-8fe4-4de412bb97f9)

### Software 
* **Communication:** 
  In order to have Jetson Nano and STM32 talk to each other bi directionally, we used a customed UART protocol from ROS package called 'Rosserial.' Rosserial protocol was able to give us the ncecesity we need to transfer data from each other while autonomy. With rosserial, STM32 became its own node and Jetson Nano has its main node and sub node. Nodes are basically scripts, which transfer myriad of data types from node to node.
   
