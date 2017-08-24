# operating-System
homework and project for course operating system

•	Implement a character device driver for producer-consumer problem using C in Linux, synchronize the multiple input and output process in a limited buffer using counting semaphore. 
Write shell script to load, unload and test the device.

-用c语言在linux中实现解决多进程生产者消费者问题。在有限缓存空间使用计数信号量同步多个输入输出进程。<br>
-编写shell脚本加载，卸载，测试驱动。


Compiling and installing the scullpipe devices:
--------------------------------------------------
1. Execute Make: 
   make
2. If scull devices already exist then unload them first.
   sudo ./scull_unload
   This may ask you for your root password.
3. Load scull devices
   sudo ./scull_load
   This will create four scull devices (scull0, scull1, scull2, scull3), and four
   scullpipe devices (scullpipe0, scullpipe1, scullpipe2, scullpipe3). 
4. Check that devices are properly created.
   ls -ltr /dev/scull*
5. Change device permissions to 777
   sudo chmod 777 /dev/scull*

Testing the functioning of scull pipe devices:
-------------------------------------------------
- Open two terminals corresponding to two consumer processes.
- On these terminal windows execute the following command: more /dev/scullpipe0
  Both these processes will block as there is no data available in the pipe yet.
- Open another terminal corresponding to the producer process. 
  From this terminal window execute the following commands repeatedly:
  echo "String 1" > /dev/scullpipe0
  echo "String 2" > /dev/scullpipe0
- You will see that each string is consumed by only one consumer.
  Once a string is consumed, it is not available for other consumer.
