## fast_copy

Fast copy utility using C++20 threads library.

### How to build

To built you need to have cmake and some compiler at your computer.
```sh
git clone https://github.com/opynao/fast_copy  
cd fast_copy  
mkdir __build  
cmake ..  
cmake --build .  
```

### Features
* Very fast (three times faster than standard **cp** utility in Linux)
* Supports backup mode
* Support loading configuration from file (.json and .ini)
* Allows you to filter files by mask using regex and to add extension to copied files 

### Options available
```sh
Usage: cp [OPTION]... --source SOURCE --destination DEST
Copy SOURCE to DEST.

Mandatory options:
-s, --source /path/to/source/directory
-d, --destination /path/to/destination/directory

Nonmandatory options:
-c, --config /path/to/configuration/file.json     the utility can parse .json and .ini.
--mode [MODE]                                     supports two mode: regular and backup. Default is regular.
-t, --threads [THREADS NUMBER]                    the default value is the number of concurrent threads supported by the implementation.
-n, --mask [FILENAME MASK]                        filter files with mask using regex. The default value is "."
-a, --addext [ADDITIONAL EXTENSION]               adding extension to copied files. The default value is ""
-l, --loglevel [LOG LEVEL]                        supports three log levels of console: info, warning and error. The default log level of file is info. The default log level of console is error.
-p, --periodicity [PERIODICITY TIME]              mandatory for backup mode. Takes parameter in seconds.The default value is 60.
```
### Configuration .ini example
```sh
source = /path/to/source/directory
destination = /path/to/destination/directory
mode = backup
periodicity = 10
threads = 6
addext = .swp
mask = .
loglevel = info
```
### Configuration .json example
```sh
{
"source" : "/path/to/source/directory",
"destination" : "/path/to/destination/directory",
"mode" : "regular",
"periodicity" : "1000",
"threads" : "6",
"addext" : ".swp",
"mask" : ".",
"loglevel" : "info"
}
```