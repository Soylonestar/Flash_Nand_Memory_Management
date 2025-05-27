# FLASH_MEMORY_MANAGEMENT

To communicate with the inbuilt FLASH NAND 4GB (Flash Memory) on our boards. Which we'll use the SPI (Serial Peripheral Interface). But we'll see where it goes. <br/>

The board that I'll be using is V2.2 board but, for testing purposes I'll primarily be using V2.1 board. Just keep that in mind. <br/>

Update: <br/>
Mostly using the V2.2 board. <br/>

1) Understanding how SPI works.
2) Make sure that SPI operates correctly.
3) Make a multi-slave system. <br/>
   a.) With the Mega_1284P board <br/>
   b.) With the Flash_Nand Memory on the V2.2 board <br/>

IMPORTANT: MUST MAKE SURE OF ADEQUATE GROUNDING OF BOTH THE MASTER AND SLAVE, OTHERWISE SPI WILL NOT WORK CORRECTLY!!! <br/>

[ATmega 2560 Datasheet](https://ww1.microchip.com/downloads/en/devicedoc/atmel-2549-8-bit-avr-microcontroller-atmega640-1280-1281-2560-2561_datasheet.pdf) <br/>

## Updates

7/6/24 <br/>
Learned that the communication is with SPI protocol, which is different to I2C. So something new... <br/>

[Helpful Link but not the actual hardware](https://www.mouser.com/catalog/specsheets/micts05893-1.pdf) <br/>

7/8/2024 <br/>
Got some code going, USART, etc. <br/>

7/9/2024 <br/>
Got my hands the Actual Datasheet of the Flash Nand Memory chip: <br/>
[Flash memory 4GB NAND_Micron Technology_MT29F4G01ADAGDWB-ITG TR .pdf](https://github.com/user-attachments/files/16145512/Flash.memory.4GB.NAND_Micron.Technology_MT29F4G01ADAGDWB-ITG.TR.pdf) <br/>

Here are helpful tutorial about SPI and Serial Memory: <br/>
[Interfacing AVR Microcontrollers with Serial Memories](https://ww1.microchip.com/downloads/aemDocuments/documents/OTH/ApplicationNotes/ApplicationNotes/AN2665-Interfacing-AVR-MC-with-Serial-Memories-00002665A.pdf) <br/>
[C Functions for Reading and Writing to Flash Memory](https://ww1.microchip.com/downloads/aemDocuments/documents/OTH/ApplicationNotes/ApplicationNotes/Atmel-2575-C-Functions-for-Reading-and-Writing-to-Flash-Memory_ApplicationNote_AVR106.pdf) <br/>
[Setup and Use of the SPI](https://ww1.microchip.com/downloads/en/AppNotes/Atmel-2585-Setup-and-Use-of-the-SPI_ApplicationNote_AVR151.pdf) <br/>

Here is the schmatics of the V2.1 board's FLASH_NAND_4GB memory: <br/>
<img src = "https://github.com/Soylonestar/FLASH_MEMORY_MANAGEMENT/assets/53907747/3b274faf-1f40-4f80-bf29-480e39c2bfda" width = "500" /> <br/>

Another schmatics of the V2.1 for the MOSI, MISO, etc. pins before goinginto the FLASH_NAND_4GB memory. Make sure that the Voltage Translator is operating correctly. Code may be necessary. <br/>
<img src = "https://github.com/Soylonestar/FLASH_MEMORY_MANAGEMENT/assets/53907747/c1611855-21d5-4131-8566-4682743f0e0f" width = "600" /> <br/>

Another schmatics of the V2.1 for CS_FLASH before going into the FLASH_NAND_4GB memory: <br/>
<img src = "https://github.com/Soylonestar/FLASH_MEMORY_MANAGEMENT/assets/53907747/539812e4-ec1e-4397-8966-24bf13ae0123" width = "500" /> <br/>

7/10/2024 <br/>
Helpful SPI Tutorial: <br/>
[ElectronicWings SPI in AVR](https://www.electronicwings.com/avr-atmega/atmega1632-spi) <br/>

7/11/2024 <br/>
Have the SPI protocol initialized...<br/>

7/12/2024 <br/>
Starting to figure out how to write into Flash NAND memory, I'm treating SPI as USART on how it writes into its buffer. <br/>
[AVR Basics: SPI on the ATMEGA - Part 2](https://mansfield-devine.com/speculatrix/2018/01/avr-basics-spi-on-the-atmega-part-2/) <br/>

7/16/2024 <br/>
Working on determing the process of reading/writing data...<br/>
The Timing diagrams of the Flash Nand tells exactly the process and bytes that must be sent in order to get the process right!!!!<br/>
<img src = "https://github.com/user-attachments/assets/a661ccdc-b59e-4384-bbb2-917d6ab87ad5" width = "500"> <br/>

7/17/2024 <br/>
Finishing on write operations and going to start read operations...also created a header file for all the commands for Flash_NAND...<br/>
Ok I think all operations are set...Let's just hope that it works...<br/>

Update: <br/>
Surprise...it don't work...currently trying to narrow down why it's getting stuck. At the moment, I know it's stuck on the Flash_Write_Data function. Where is a good question. <br/>

7/18/24 <br/>
Narrowed it down to the status' while loop...but I noticed that it's spewing random data so gotta check that out...<br/>

Update: <br/>
Created two header files, one for Board V2.1 operations, and the other for Board V2.2 operations.<br/>

Another thing I got to do is focus on doing testing on V2.2 boards, so here are the schematics for what is used...<br/>

Here is how the ~CS is regulated, so I may need to do something with the Voltage Translator (software wise) in order to make sure that the ~CS is working correctly. <br/>
<img src = "https://github.com/user-attachments/assets/39cdd034-8fc0-43ce-a860-8868d958af54" width = "500"> <br/>

Here is the FLASH Nand memory chip on the V2.2 Board. Different names but same inputs and outputs. <br/>
<img src = "https://github.com/user-attachments/assets/58ededb0-534b-44f3-a7c7-32a2906bf3e4" width = "500"> <br/>

Here is the POCI, PICO, SCK data lines onto Voltage Translator. Software may be necessary in order to make sure that it's operating correctly. <br/>
<img src = "https://github.com/user-attachments/assets/d2b963ba-fc85-4ecb-9c9d-4b7811fb59d5" width = "500"> <br/>

7/19/2024 <br/>
Voltage Translator for SPI data lines must be set to THREE-STATE for normal operations. <br/>
<img src= "https://github.com/user-attachments/assets/904bfc8b-aa2c-445b-bd6c-a8f9aff38dae" width="500"> <br/>

Voltage Translator for ~CS...EDIT HERE ASIDHJFLAJDHJFDIOSDFHOSDHFO different voltage translator<br/>
<img src= "https://github.com/user-attachments/assets/5af39958-caff-4d0b-9a5f-0c83a7ffc2a7" width = "500"> <br/>

This is the data we are seeing from SPI operations...seems to do something but not working...<br/>
<img src = "https://github.com/user-attachments/assets/e898abe0-c14e-4b79-9dd6-804eb517c6a0" width = "500"> <br/>

Here is, maybe, a helpful about SPI Flash Memory: <br/>
[ATMega 328 + SPI Flash Memory](https://forum.arduino.cc/t/atmega328-spi-flash-memory/45108) <br/>

Update: <br/>
We are going to try the SPI protocol with another board to make sure that the functions work...<br/>
Board that we are working on is the XMEGA - 1284P XPLAINED: <br/>
<img src = "https://github.com/user-attachments/assets/67affacd-45ef-4c59-b121-76b0667bf3c4" width = "500"> <br/>

[Mega 1284P Datasheet](https://ww1.microchip.com/downloads/aemDocuments/documents/MCU08/ProductDocuments/DataSheets/ATmega164A_PA-324A_PA-644A_PA-1284_P_Data-Sheet-40002070B.pdf) <br/>
[Mega 1284P Hardware User's Guide](https://ww1.microchip.com/downloads/en/Appnotes/doc8377.pdf) <br/>

7/23/2024 <br/>
Not using the XMEGA-A3BU because of the lack of register data on datasheet. So I'm going to use the MEGA 1284P board because of the very detailed data sheet. And I also know how it works...for the most part. <br/>

Update: <br/>
Here is the type of communication we are doing: <br/>
<img src= "https://github.com/user-attachments/assets/12013ce1-0df8-4879-adc2-66c02fd066a5" width = "500" > <br/>
It's the same SPI we've been dealing with since FLASH_NAND_Memory stuff...but instead haveing to send specific command bytes etc for the Flash NAND memory. We are just sending data to the Slave (MEGA_1284P) and vice versa (ideally)...sending and clearing flags are the same. Just the process of sending data and recieving are slightly different becasue of what we are communicating with. <br/>

7/24/2024 <br/>
Getting something but the Slave device isn't reading the same values seen being sent from the Master. <br/>
Master is correct, data is correct. On the Atmega 2560 transmit the correct data that I want it to send to slave. But, slave right now, isn't receiving that data correctly...<br/>

7/25/2024 <br/>
SOOO I found out that I was fucking stupid and didn't have the SPI wires correctly placed on the V2.2 ISP port.... fuck. Anyway, getting some data now, but Slave still deviates from the correct data (1, in this case)...so still figuring it out but closer....<br/>

Here is the data comming out of the Master: <br/>
<img src= "https://github.com/user-attachments/assets/da47e27f-8c7d-4f64-8185-c961c8745170" width = "500" > <br/>

Here is what I'm working with on the SLAVE: <br/>
<img src = "https://github.com/user-attachments/assets/a632a1ba-c774-497a-a3cb-72e689be27fc" width = "400" > <br/>

7/26/2024 <br/>
Here is a helpful link, atleast the code is interesting for sending strings to slave and back...<br/>
[Master-Slave SPI Communication](https://www.makerguides.com/master-slave-spi-communication-arduino/) <br/>

Update: <br/>
IT FUCKING WOORRRRKSSS!!!!! Turns out that any static on the SPI line will cause error to accumalate. SO you gotta make sure to GROUND THE CONNNECTIONS!!! <br/>
Although keep in mind of timing issues...if I don't maually turn SPI on/off after each set of data, it kinda bugs the fuck out...so gotta figure out why....<br/>

Here is what I'm seeing: <br/>
<img src = "https://github.com/user-attachments/assets/4a25c77d-9e6a-4646-9552-10e5031cbb56" width = "500" > <br/>

Update: <br/>
Now I'm currently working on being able to send a string over SPI. <br/>

7/29/2024 <br/>
Got the string data to send accurately...the reason was the same, GROUNDING ISSUES. Gotta make sure that both MASTER and SLAVE is GROUNDED enough...<br/>

Here is what I'm getting now, from MASTER to SLAVE: <br/>
<img  src = "https://github.com/user-attachments/assets/e5b54b29-b751-4b78-bb53-35c94afd9203" width = "500" > <br/>

7/30/2024 <br/>
Important Note: In order for the Slave to talk to the Master, the roles must flip. The Mega 1284P must be converted into Master, and Atmega 2560 must be converted to Slave. This is because the Master is the only one that transmits CLK. <br/>
So instead of doing all of that, which should work at the end of the day, I'm going back to the FLASH NAND Memory stuff again on the V2.2 board. <br/> 

Update: <br/>
So...Flash NAND commands seems to be processing, and WE ARE getting something back from the read function, although accompanied with a lot of reverse "???". And a lot of hit or miss.... Allocation and addressing issue? IDK...<br/>
<img src = "https://github.com/user-attachments/assets/be315785-181a-404c-92ae-4d390f68c632" width = "500"> <br/>

7/31/2024 <br/>
Welp, today was a shit show. Went from receiving data to not receive shit....fuck <br/>
[FLASH Memory 101: An introduction to Nand Flash](https://www.edn.com/flash-memory-101-an-introduction-to-nand-flash/) <br/>

8/1/2024 <br/>
Today was an experiment, deceided on obtaining the device ID from the Flash Nand...it works.<br/>
Now, I'm going to see if I can get data from the Parameter Page....<br/>

Here is the Device ID, the first hex is Status, the second hex is a dummy byte returned, the third hex is the Micron ID, and the fourth hex is the 4Gb 3.3V Device ID. <br/>
<img src = "https://github.com/user-attachments/assets/b153117d-82f4-45e8-b00f-9c9101a98e41" width = "500"> <br/>

8/2/2024 <br/>
Progress progress progress, currently running some diagnostic code that I made to see what bytes of importance I'm getting back...currently I'm getting 0x0C in the Status Register, which shows me on a table that I'm getting Program Fail and Erase Fail. <br/>
<img src = "https://github.com/user-attachments/assets/fafdd2df-9649-4344-9dde-899191ae3df2" width = "500"> <br/>

Here is the table I'm talking about...<br/>
<img src = "https://github.com/user-attachments/assets/7174b976-94ca-48dc-95a3-cd7890c66664" width = "500"> <br/>

IT WOOORKKKSSSSS!!!! The plane bit, set that shit to 1 and it worked. <br/>
<img src = "https://github.com/user-attachments/assets/38fbd3a8-0313-42eb-964c-bee309b45ccb" width = "500"> <br/>

8/5/2024 <br/>
So It still works, currently just cleaning up and making sure the code itself works. ALTHOUGH we still are getting the same error after turning off the V2.2 Board. It takes a while to be able to talk to the Flash NAND again. I'm not sure why...Hopefully as I continue on working on gettting this code fully functional and useable, that power off state will not be a problem anymore. Hopefully... <br/>

Update: <br/>
Got some Receive UART Code working...need to create somesort of library for Command Lines...<br/>

8/6/2024 <br/>
Made some serious file management adjustment to the code. This include new folders, with either header files and c files. Along with the main.c file as the main. <br/>
Everything works, except when user input writes into memory...still figuring that part out...<br/>

8/7/2024 <br/>
So I think I found out that, clearing the array by setting it to zero, or null. Isn't the way to do things...I guess.<br/>
I don't think that's the problem though, it's weird how when I read memory, everything is fine, but when I write to memory things start to act up...<br/>
<img src = "https://github.com/user-attachments/assets/6faf6bea-fcd1-483e-a98a-5de912c22966" width = "500" > <br/>

Update: <br/>
Here is what I'm getting when Using the CommandBuffer for my Write_Data in Flash function <br/>
<img src =  "https://github.com/user-attachments/assets/a0814f9b-f9e1-412e-b50c-955edccda670" width = "500" > <br/>

8/9/2024 <br/>
Started adding the parameter page feature onto this code to make sure that everything works...<br/>
<img src = "https://github.com/user-attachments/assets/6dbe068b-642e-4562-818f-c011fdb4ed6e" width = "500" > <br/>
<img src = "https://github.com/user-attachments/assets/bc657abd-5f2e-42c3-8c1f-241b18528fcc" width = "500" > <br/>
<img src = "https://github.com/user-attachments/assets/c701d43f-f4d5-4ded-9452-fc4e5f96747e" width = "500" > <br/>

8/12/2024 <br/>
Here is the Registers of the interests...<br/>
<img src = "https://github.com/user-attachments/assets/cc3adf8d-b288-4817-8021-74e2e871ca70" width = "500" > <br/>
<img src = "https://github.com/user-attachments/assets/e3e79815-d49a-4754-828a-3a88b53af8a0" width = "500" > <br/>

8/13/2024 <br/>
Gotta figure out why this Flash Nand does not like being turned off, and then take forever to work correctly again. <br/>
The only thing I changed was the delay (to 125 us) in the Flash ID, and added the following functions into the while(1) loop:
1) //CLEAR_ARR(); <br/>
2) //FLASH_ID(); <br/>
3) //FLASH_Write_Data(); <br/>
4)	//FLASH_Read(); <br/>

Keep on screwing the delay of the Flash ID until I started receiving the correct Device ID values, and now everything works again...<br.>

Update: <br/>
So simplified code with calling some Flash_Status() function, and also made two functions (atleast for the meantime) that makes sure that Cache and MainArray address are where they should be for normal write/read operation but also for Paramter page operation. <br/>

8/14/2024 <br/>
Parameter page works, correclty outputs the expected address at correct byte number...<br/>

Update: <br/>
IT WORRKS!!!! I can fully read/write to FLASH NAND...<br/>
<img src = "https://github.com/user-attachments/assets/5c74a541-c1c1-4c26-a516-d0da9aebdd86" width = "500" > <br/>

8/15/2024 <br/>
Now that, I can read/write to FLASH NAND, I gotta figure out how to subdivide memory space, how to allocate memory via blocks, plane, pages, etc. <br/>

Update: <br/>
Got newline to work, but it involve changing what is sent on enter while writing in UART such as only sending '\n' which allows newline to work. But if you want to end transmission, then you have to change '\n' to '\r' in order to end transmission on enter. <br/>
Which sucks, but gonna see what I can make up to fix that...<br/>
<img src = "https://github.com/user-attachments/assets/cdfb19c3-7fb4-4a8a-8201-8517c2757c77" width = "500" > <br/>

8/28/2024 <br/>
Came back and forth, but wasn't very consistent with this...but decided to tackle the power cycle issue of Flash NAND today. <br/>
No luck with changing the delay, atleast not consistent. So Andy gave me the idea that since SPI data lines is a shared (SCK, MOSI, MISO) with multiple slave devices. It would be a good idea to deselect all ~CS (Chip Selects) slaves. Leaving only the one I'm working with. <br/>
Therefore, we gotta figure out the voltage translator, which is differnt from the THREE-STATE voltage translator talked above. For ~CS of Flash NAND and also ~CS of ADS. Then make sure all DDR are set to output and high to deselect them. Will do that tomorrow. <br/>

8/29/2024 <br/>
Came back to this and started doing what I said I was gonna do. Currently have all slave ~CS set as output and high (deselected). Currently working making sure the corresponding voltage translators work as intended. <br/>

Update:<br/>
Here is the Voltage Translator for SPI data lines, for normal operation the Three-State pin must be set logic high. So that's been done correctly for a while now. <br/>
<img src = "https://github.com/Soylonestar/FLASH_MEMORY_MANAGEMENT/assets/53907747/c1611855-21d5-4131-8566-4682743f0e0f" width = "600" /> <br/>

For the following Voltage Translators (which are the same) here is what I found in its datasheet: <br/>
[TXU0104 Voltage Translator Datasheet](https://www.ti.com/lit/ds/symlink/txu0104.pdf?ts=1724919882985&ref_url=https%253A%252F%252Fwww.mouser.com%252F) <br/>
Which means that the OE pin must be set to logic high in order to get normal operations. <br/>
<img src = "https://github.com/user-attachments/assets/70d19620-d733-446a-be0e-ead7739c3f9e" width = "500"> <br/>

Here is the Voltage Translator for ~CS_FLASH,  <br/>
<img src = "https://github.com/user-attachments/assets/5af39958-caff-4d0b-9a5f-0c83a7ffc2a7" width = "500"> <br/>

Here is the Voltage Traslator for ~CS_SD and ~CS_ADS, <br/>
<img src = "https://github.com/user-attachments/assets/6aa447b6-1edd-4d60-9e09-444d2309bb0b" width = "500"> <br/>

Update: <br/>
I'm going to keep the code, because it remove floating pins. But this still hasn't fixed the problem...<br/>

9/26/2024 <br/>
I've been busy with other stuff. So slowly getting back to this, I've been tinkering with the timing. Delays, spi timing, etc. But need to keep working on it...<br/>

1/9/2025 <br/>
So starting 1/6 to today, I've expected the code to work to begin with, but for some odd reason the code only accepted three characters. Not sure if the "If"-Statement had anything to do with that. It made more sense to put it in a "while" loop instead. But it came to a point that I just copied the UserInput function to the CommandLine function and change a couple of things and boom works. <br/>
Also another important thing, Board 4 of the V2.2 doesn't work. It must've been shot with static built up, and just kind of not work anymore. I wouldn't be surprised if this were the underlying issue into why the flash nand on that board didn't work. Because of some noise and grounding issue with the spi terminals. <br/>
So currently I'm using the Board 000 of the V2.2 with its Flash Nand. Although while the code appears to work (all except the Erase Memory function, will work on that later), it is imporant to note that the flash nand on this board is slightly different than the previous.<br/>
According to the Flash ID function, it's id is "Micron ID (0x2C) Device ID (0x24)". Which means the following: <br/>
<img src = "https://github.com/user-attachments/assets/3881eccf-e46e-43bf-94cb-deaa6589ad49" width = "600" /> <br/>
But at the moment everything works and I'm very happy with it. Now I'm going to work on making sure the Memory Erase function works instead of having to reset the device all together. <br/>

5/13/2025 <br/>
Going back to understand what I wrote for this Flash NAND Driver. <br/>
At the moment, everything works, currently with the 000 board. V2.2. I even commented out all the delays to make the process a little bit quicker.
Everything works just fine, but currently trying to determine why the status registers reads 0x08 which complies with Program_Fail (P_Fail). But everythign works...so what could be triggering this...<br/>
I also just read my last log, I will look into the Memory Erase Function later.<br/>
Also just for manangement reason, I got git bash to work with my local laptop and my working computer, so I should be able to work on this anywhere. Able to commit and pull if new contents. <br/>

5/14/2025 <br/>
Going bit by bit to fully understand where I'm writing data, as that could very well be the reason as to why I'm not erasing data...<br/>
Turns out, my original function I had called Flash_Cache_Address is not a Cache Address but a Column Address, for memory....dumb fuck<br/>
I have almost a full grasp on how this memory stuff is allocated and divided in the Flash NAND... which is awesome. <br/>

5/15/25 <br/>
Continuing on yesterdays ideas. <br/>
Starting to work around the idea of using the Erase Block thing. <br/>
I really truly didn't understand NAND till now, basically in order to write into the Flash, I need to erase (reset to 1) the block I want to write to, then program it (change specific bits to 0). Which is interesting. This could very well be the reason why P_Fail occurs. <br/>
I'm starting to show different status stuff, so something is happening. <br/>
But, erasing still doesn't work and writing correctly into block also doesn't work right now. But progress is progress. <br/>

5/20/25 <br/>
So I only changed the block/page back to block 0 and page 0 (also 1, which is the parameter page) although I still get P_Fail in the status register. I can write and read...only until the power is off. Then everything erases...odd. <br/>
Added a new operation, Flash Status, which only shows me the status register. Might be helpful...adding some delay operations with do-while...<br/>
I added to polling do-while, that works. But the whole idea, still doesn't work. I even added something to check the block (Block Checker)...still unsure if that even works though...quite uneventful...<br/>
No progress again...I'm running out of ideas...<br/>

5/21/25 <br/>
Decided to make the debugging process a little easier on me. So I changed my uart to accept uint8_t instead of char, and started on working on obtaining hex values based on my inputs. <br/>
I do have a feature that prints out my hex numbers to make sure it correlates...hopefully I can use that value directly...<br/>
Almost have it done entirely, then after that we'll need to change a couple of functions according to the byte addresses. <br/>
Hopefully this will make the debugging process a little easier.<br/>

5/22/25 <br/>
This is what I have at the moment, I got the last 6 hexs work, but the first 4 hexs don't work yet...<br/>
<img src = "https://github.com/user-attachments/assets/6e6debec-2a6c-4797-8244-0237c5cf97a1" width = "600" /> <br/>

This is what this looks like right now...kinda not ideal. I need to show 0x83 instead of 0x08 and 0x03. To actually apply this in the SPDR.<br/>
<img src = "https://github.com/user-attachments/assets/195a1230-4196-440f-b045-9a09983c6cfc" width = "600" />

5/27/25 <br/>
Going to fix the hex stuff today, shouldn't be too much of a hassle. <br/>
Alright, took longer than expected, but kinda got to fixing other things while progressing in fixing the hex conversion and representation. <br/>
But right now, that works...look <br/>
<img src = "https://github.com/user-attachments/assets/a86a8e4d-7cb8-425a-978f-fbac953c3780" width = "500"/> <br/>
Created a couple of new functions, simplified a couple of methods. But right now, the hex values seem to work. Not to long, but I'll go back to debugging the Flash NAND again in a few. <br/>
Going to create a test method, that iterates through one entire block (or maybe the whole flash, we'll see), and see what block/pages are valid...<br/>

