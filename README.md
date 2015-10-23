# psuspend
Process suspend/resume tool for win32 (built with VS2013 CE)

##Usage
```
psuspend <option> <pid1> <pid2> ...

Where optins are:
 
 [Threads operations]
-ts Threads Suspnde
-tr Threads Resume
 
 [NT hack (undoc API XP+)]
-ns NT Suspend
-nr NT Resume

 [Debugger]
-ds Debugger Start
-dr Debugger Resume
```
