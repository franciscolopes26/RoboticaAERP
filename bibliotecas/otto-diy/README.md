# Otto DIY




**Background information:**

​

Otto is truly Opensource; it means the hardware is easily discerned so that others can make it, is also Arduino compatible, 3D printable and customizable, the perfect opportunity to build and have your very first robot, learn robotics and have fun, you will learn the logical connection between code and action, and by assembling it, you will understand how its components and electronics work. 

​

Visit www.ottodiy.com for more detailed info.

​

### **Build Your Own Robot**

> ### Otto DIY can be made with [Builder Kit \(full with 3D printed parts\)](https://ottodiy.ecwid.com/Otto-DIY-Builder-Kit-p135022769)  and [Maker Kit \(only electronics\)](https://ottodiy.ecwid.com/Otto-DIY-Maker-Kit-p135022782) or create your own project with this open source content.




## How it Works
















@[youtube](https://youtube.com/embed/VD6sgTo6NOY)

​

### Otto walks, dances, makes sounds and avoids obstacles, easy to program and expand or customize by your own.

![undefined](https://alpha.wikifactory.com/files/RmlsZTo0NDg5)

​

​
















## Part List

Here there is a list of the materials you will need to build Otto.

![image](https://wikifactory.com/files/RmlsZToyMDY4NDY%3D)

* 1 x Nano ATmega328
* 1 x Nano I/O shield
* 1 x USB-A to Mini-USB Cable
* 1 x Ultrasound sensor HC-SR04
* 4 x Micro servo SG90 9g
* 1 x Piezo Buzzer
* 6 x Female/Female Dupont Wires
* 1 x AAbattery holder
* 4 x AA Batteries Alkaline \(must be new or have 1.5V each\)
* 1 x Micro Switch Self lock On/Off 8x8mm
* **3D printed head**

[View OttoDIY_head_V9.stl @ Wikifactory](https://wikifactory.com/+OttoDIY/otto-diy/file/3Dprint/OttoDIY_head_V9.stl)

* **3D printed body**

[View OttoDIY_body_V9.stl @ Wikifactory](https://wikifactory.com/+OttoDIY/otto-diy/file/3Dprint/OttoDIY_body_V9.stl)

* **3D printed leg x2**

[View OttoDIY_Leg_V9.stl @ Wikifactory](https://wikifactory.com/+OttoDIY/otto-diy/file/3Dprint/OttoDIY_Leg_V9.stl)

* **3D printed foot x2**

[View OttoDIY_foot_V9.stl @ Wikifactory](https://wikifactory.com/+OttoDIY/otto-diy/file/3Dprint/OttoDIY_foot_V9.stl)

**Tools:**

* Small Phillips screwdriver.
* Computer.
* Scissors or pliers.

## 3D Printing




If you bought a **Builder kit** you can skip ahead but if you have the **Maker kit** you have to[ 3D print the parts ](https://wikifactory.com/+OttoDIY/otto-diy/files/3Dprint)but Otto is very well designed for 3D printing, so wont give you trouble if you follow this common parameters:

* Recommended to use a FDM 3D printer with PLA material.
* No need supports or rafts.
* Resolution: 0.20mm
* Fill density 20%

​

All parts in total use 115gr aprox. equivalent to 14.5m of regular PLA 1.75mm thickness, it should take around **8 hours to 3D print** a full set of parts for one Otto even less with a proper 3D printer and optimized settings.




## Software

### For **beginners it** is recommended to start coding by using our new Blockly a simple visual programming software:

Otto Blockly is completely autonomous \(no need for Arduino IDE, or libraries setup and no Internet access is required\).

![image](https://wikifactory.com/files/RmlsZToyODQ0MjU=)

1. Download [](http://www.mblock.cc/mblock-software/)from our website here \(https://www.ottodiy.com/#blockly-and-app\)
2. Unzip and open OttoBlockly.
3. Open an example.
4. Connect your Otto robot.
5. Select Arduino nano, **USB port where Otto is connected.**
6. Check the code.
7. Upload and yes is that easy!

@[youtube](https://youtube.com/embed/chcWxh4Co_c)

**If your computer did not recognize the USB device you should install the driver CH340 for your Operative System find it here:** [https://sparks.gogo.co.nz/ch340.html](https://sparks.gogo.co.nz/ch340.html)

​

Play with the examples and start mixing blocks in multiple ways to decide the behaviour of your robot and generate your own codes.

\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_\_

​

### For **more advanced** Arduino programming follow our [coding guide](https://wikifactory.com/+OttoDIY/otto-diy/files/Instruction%20manual) from the files tab folder, it will allow you to really exploit the potential of your robot, you will be able to try more than 15 examples sketchs that you can just simply upload and modify the source for your own ideas.

1. Download the software for free here:[ Arduino IDE \(version 1.8.5\) https://www.arduino.cc/en/Main/OldSoftwareReleases\#previous](https://www.arduino.cc/en/Main/OldSoftwareReleases#previous)
2. InstallArduino IDE software.
3. Download all the libraries from the files tab and unzip in your libraries folder \(for example C:\Users\user\Documents\Arduino\libraries\)
4. Connect your Arduino Nano through USB make sure the driver CH340 is installed in your computer by checking is in COM\#
5. Open one of the code examples[ in the code files tab folder](https://wikifactory.com/+OttoDIY/otto-diy/files/Code)
6. Upload the code and check that everything is working.
   [View OttoDIY_codingguide_V9.pdf @ Wikifactory](https://wikifactory.com/+OttoDIY/otto-diy/file/Instruction%20manual/OttoDIY_codingguide_V9.pdf)

## Electronics




### **Before inserting the AA batteries you should check your connections and test your robot with the USB cable connected to a computer.**

As a good practice you should check your electronics and software in your computer, before assembling all the robot, this avoid having to disassemble the whole robot to fix something. You can do this by just connecting all the servos, in the place indincated in the wring below **and upload any code** that makes Otto move with Otto Blockly or Arduino.

![image](https://wikifactory.com/files/RmlsZToyODQ0Mjg=)

​

​

If you feel ready for Batteries you can also check if your power source works, depending of the type of cells you have there can be 2 options:

![image](https://wikifactory.com/files/RmlsZToyODQ0MjY=)

![image](https://wikifactory.com/files/RmlsZToyODQ0Mjc=)

​

There are actually plenty options to power your Otto robot in multiple ways, we explore in more detail in this blog post article: https://www.ottodiy.com/blog/power




## Assembly Instructions

Gather all the off the shelf hardware parts that you'll need for this assembly, or start with our full kits which come with all parts needed. Kits are available from our website www.ottodiy.com

​

Before getting started with the assembly, download and read carefully this [PDF from the instruction manuals files tab folder.](https://wikifactory.com/+OttoDIY/otto-diy/files/Instruction%20manual)

[View OttoDIY_Manual_V9.pdf @ Wikifactory](https://wikifactory.com/+OttoDIY/otto-diy/file/Instruction%20manual/OttoDIY_Manual_V9.pdf)

​

If you do not understand something in the manual you can just refer to this video that shows how to build Otto.

@[youtube](https://youtube.com/embed/34v7R0FrSNE)

​

If you see some missaligment in the movements, double check that you centered the arms repect to your servos before fixing with the short screw, but if you want more precise walk and movements, then you can do some electronic calibration as we explain in detail in this blog article: https://www.ottodiy.com/blog/calibration

## Customize

Download, print, cut and make papercrafts of Otto to personalize your robot.

[https://wikifactory.com/+OttoDIY/otto-diy/files/Papercraft](https://wikifactory.com/+OttoDIY/otto-diy/files/Papercraft)

[View OttoDIY_papercraft_milky_A4.png @ Wikifactory](https://wikifactory.com/+OttoDIY/otto-diy/file/Papercraft/OttoDIY_papercraft_milky_A4.png)

Or if you wanna learn 3D modeling Otto is design using [TinkerCAD ](https://www.tinkercad.com/users/h8O1zJQboeH-camilo-parra-palacio)a FREE 3D modeling online software! , you can modify Otto parts for further improvements or create new accessories for new features.

[View OttoDIY_v03.stp @ Wikifactory](https://wikifactory.com/+OttoDIY/otto-diy/file/CAD/OttoDIY_v03.stp)

[View Otto_glasses_code.stl @ Wikifactory](https://wikifactory.com/+OttoDIY/otto-diy/file/3Daccessories/Otto_glasses_code.stl)

![image](https://wikifactory.com/files/RmlsZToyNDE2NzM%3D)

Read this blog post to learn more options of how to redesign your own robot [https://www.ottodiy.com/blog/design](https://www.ottodiy.com/blog/design)

## Join the Otto Builder community!




Follow us, subscribe, give us a like and share your creativity.

​

1. [Join us here in Wikifactory](https://wikifactory.com/invite/SW52aXRlTGluazo3Ng/ZzHGYHg9oAwik0u-vJrz-x0fsEWrONV54SaYpNx9t1A) & check our [Forum](https://wikifactory.com/+OttoDIY/forum) for questions.
2. [Group in Facebook](https://www.facebook.com/groups/ottodiy/). to share and ask for help in the community and like our [Facebook page](https://www.facebook.com/ottodiy/)
3. [YouTube channel](https://www.youtube.com/c/ottodiy?sub_confirmation=1) for more how to videos and tutorials.
4. [Instagram](https://www.instagram.com/ottodiy/) to share \#ottodiy
5. [Twitter](https://twitter.com/ottodiy) to share \#ottodiy

​

Be a part of this friendly community of robot builders, teachers and makers!

​

Welcome to our Otto Builder community!




## License CC-BY-SA




Otto DIY by www.ottodiy.com is licensed under a Creative Commons Attribution-ShareAlike 4.0 International License.

​

You can use all resources of Otto for free but Otto DIY Website must be included in any redistribution and remixes must keep the CC-BY-SA license, As you might seen with the original Otto DIY project the idea is that more people can have access, therefore if you copy or remix Otto you must also release under same open license, that means you must release all files also free to the public.