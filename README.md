# About
> Program to terminate every instance of unwanted processes.<br>
> Orginally made to terminate malicious (for me and classmates) scripts on school laptops.<br>
> I couldn't find any project like, so I made this.<br>
> This is my first functioning program in ```C++```<br>
> First versions: https://github.com/JamJestJerzy/ProcessKiller
# Usage
> - Open `terminal`/`powershell`/`cmd` in directory with Program's binary
> - Run binary from console (```{name}.exe```) with positional argument ```delay - Delay between scans.```:
> ```console
> PS C:\Users\JamJestJerzy> .\ProcessKiller-0.0.0.0.exe 50
>    _____          __ ________
>   /  _  \________/  |\_____  \ __ __  ______________   ____   ____     _____   ___________   ____
>  /  /_\  \_  __ \   __\_(__  <|  |  \/  ___/\___   /  /    \ /  _ \   /     \ /  _ \_  __ \_/ __ \
> /    |    \  | \/|  | /       \  |  /\___ \  /    /  |   |  (  <_> ) |  Y Y  (  <_> )  | \/\  ___/
> \____|__  /__|   |__|/______  /____//____  >/_____ \ |___|  /\____/  |__|_|  /\____/|__|    \___  >
>         \/                  \/           \/       \/      \/               \/                   \/
> ProcessKiller-Recode v0.0.0.0 by Jerzy W (https://github.com/JamJestJerzy)
> [!] Created config file.
> ```
> - Program will generate config file for you.
> - Fill this file with exe names (CaSe SeNsItIvE) of processes you want to terminate:
> ```properties
> # Comment
> Never.exe
> Gonna.exe
> Give.exe
> You.exe
> Up.exe
> ```
> - Run program again:
> ```console
> PS C:\Users\JamJestJerzy> .\ProcessKiller-0.0.0.0.exe 50
>    _____          __ ________
>   /  _  \________/  |\_____  \ __ __  ______________   ____   ____     _____   ___________   ____
>  /  /_\  \_  __ \   __\_(__  <|  |  \/  ___/\___   /  /    \ /  _ \   /     \ /  _ \_  __ \_/ __ \
> /    |    \  | \/|  | /       \  |  /\___ \  /    /  |   |  (  <_> ) |  Y Y  (  <_> )  | \/\  ___/
> \____|__  /__|   |__|/______  /____//____  >/_____ \ |___|  /\____/  |__|_|  /\____/|__|    \___  >
>         \/                  \/           \/       \/      \/               \/                   \/
> ProcessKiller-Recode v0.0.0.0 by Jerzy W (https://github.com/JamJestJerzy)
> Blacklisted processes:
> Never.exe
> Gonna.exe
> Give.exe
> You.exe
> Up.exe
> Scanning processes with 50ms delay.
> ```
> - Program is now ready to terminate unwanted processes!
> - You can also check what args you have avaiable by using ```--help``` flag:
> ```console
> PS C:\Users\JamJestJerzy> .\ProcessKiller-0.0.0.0.exe 50
>    _____          __ ________
>   /  _  \________/  |\_____  \ __ __  ______________   ____   ____     _____   ___________   ____
>  /  /_\  \_  __ \   __\_(__  <|  |  \/  ___/\___   /  /    \ /  _ \   /     \ /  _ \_  __ \_/ __ \
> /    |    \  | \/|  | /       \  |  /\___ \  /    /  |   |  (  <_> ) |  Y Y  (  <_> )  | \/\  ___/
> \____|__  /__|   |__|/______  /____//____  >/_____ \ |___|  /\____/  |__|_|  /\____/|__|    \___  >
>         \/                  \/           \/       \/      \/               \/                   \/
> ProcessKiller-Recode v0.0.0.0 by Jerzy W (https://github.com/JamJestJerzy)
> Usage: ProcessKiller-Recode [--help] [--version] [--debug] [--log] delay
> 
> Positional arguments:
>   delay          Delay between scans.
> 
> Optional arguments:
>   -h, --help     shows help message and exits
>   -v, --version  prints version information and exits
>   --debug        Enabled debug output
>   --log          Logs every process. Spams terminal a lot. Trust me.
> ```
# Compilation
> - Clone this repository (```git clone https://github.com/JamJestJerzy/ProcessKiller-Recode.git```)<br>
> - `cd` into cloned folder
> - Make sure you have c compiler (`g++`) installed
> - Run command ```.\build.ps1``` in Windows PowerShell
> - Compiled binary should be in ```builds``` dir with name `ProcessKiller-{current version}.exe`
## Contact
> If u have any issues, questions or propositions feel free to message me on:
> - Discord: ```dash1e```
> - E-Mail: ```jerzy@j3rzy.dev```
> - Telegram: [JamJestJerzy](https://t.me/jamjestjerzy)
## License
> [© All Rights Reserved](https://en.wikipedia.org/wiki/All_rights_reserved)