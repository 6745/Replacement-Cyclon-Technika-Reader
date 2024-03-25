# Replacement-Cyclon-Technika-Reader
	GAMES SUPPORTED: Cyclon(Boomerang Version.) DJMAX TECHNIKA 3 (CARDEMU or Cloned Dongle Version.)
 	CARDS SUPPORTED: ALL technika and cyclon cards.
	CARDS NOT SUPPORTED: NON MIFARE, BANAPASS, AIME. (The block for reading is not located in the same spot)
# BUILDING THE READER:
	Firstly you will need a Ardiuno, if you can solder: nano, if you cannot:uno
 	You will need a RC522 RFID module, you can get these presoldered from most online stores,
  	Amazon ones usually are not however always check the listing
	Download ardiuno IDE and configure it to your boards specifications, 
 	Install the MFRC522 libary from the Library Manager
 	Wire up and match the following pinouts
![image](https://user-images.githubusercontent.com/75388599/220476085-4f6be78f-1bbe-407a-97b3-408ecee39cb0.png)
![image](https://img.youtube.com/vi/TJJ_1LiDDrc/maxresdefault.jpg)

# RUNNING:
	Firstly, this will not run on windows XP please use windows 10
 	For cab use these games run on 10 with no problems.
	Download the binaries in the release section.
 	Open device manager and find which COM port is inuse by the reader
  	modify the .bat file for the game you wanting to use
	and run the bat file (make a startup short cut for automatic connection.)
# TO DO
	fix timeout bug in cyclon  
# FAQ
	Q:Why does it not hook natively to the game?
 	A:I wanted a 1 solution for all type deal, I built this reader to bypass some DRM on Chinese Repo dongles for technika that do not allow comports to be used for the reader there is no way to remove that. 
   	Q: Can I use eamusement passes? 
    	A: The RC522 is RF only at this time its not possible to use those cards.
