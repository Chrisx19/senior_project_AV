# CPP Senior Project

## December 2022 - May 2023
ROS Noetic control GPS autonomous vehicle using STM32 and Jetson Nano. 

### Mechanical 
![image](https://github.com/Chrisx19/senior_project_AV/assets/107272321/af685e61-d4f0-4aca-9ad1-e9731f64c4dd)


### Electrical
* **Initial Design**
![image](https://github.com/Chrisx19/senior_project_AV/assets/107272321/7f8c054f-1a8e-4ad8-a297-a58b69e2f1f4)


* **Final Design**
![image](https://user-images.githubusercontent.com/107272321/230009457-7026a6dd-5a4a-4efe-8010-96198955987c.png)

### Software 
* **Communication: ** In order to have Jetson Nano and STM32 talk to each other bi directionally, we used a customed UART protocol from ROS package called 'Rosserial.' Rosserial protocol was able to give us the ncecesity we need to transfer data from each other while autonomy. With rosserial, STM32 became its own node and Jetson Nano has its main node and sub node. Nodes are basically scripts, which transfer myriad of data types from node to node. 
