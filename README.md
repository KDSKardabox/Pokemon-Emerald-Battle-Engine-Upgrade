NOTE: I recommend switching to decomps (pokeemerald). And use the new version of battle_engine suited to decomps. https://github.com/DizzyEggg/pokeemerald/tree/battle_engine_v2

# Pokemon-Emerald-Battle-Engine-Upgrade (Outdated)

Installation instructions:

1. Download devkitpro. Follow the instructions.
  (Note: you can only install devkitARM, download v45 for stability, devkit v46 tends to crash the game)

2. Download the latest version of python(3.4, 3.5 and 3.6 are alright).
  After downloading and before proceeding to install make sure that the 'add to path' checkbox is ticked, otherwise you'll have to add the python path in the environment variables manually.

3. Download the master folder from this github page
  (click 'Clone or Download', then 'Download Zip')

4. Place your rom in the main(master) folder.

5. Open a 'make.py' file in the scripts folder and configure the offset location, the rom name and additional options.

6. Run the cmd.exe in the main folder and type 'python scripts/make.py'.
  You can do this by typing 'cmd' and hitting enter in the url address or selecting 'run command prompt from here' from right                    clciking on empty space while holding the shift key

7. A new gba file will appear named as test.gba.
  This is your resultant file.

If you want to create a INI file for PGE:

8. Place a copy of a valid roms.ini in the master folder and in the cmd prompt type 'python scripts/PGEinidump.py'.
  A corresponding test.ini will be created for test.gba.
