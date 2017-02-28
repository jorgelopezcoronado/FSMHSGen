#FSMHSGen

##Compilation notes

The program can be compiled with the Makefile included in the repository, by executing ‘make’ on the code directory. The program runs with standard unix libraries. However, lex and bison must be installed in order for the program to compile. Bison version >= 2.4 is required. Mac users are required to install a newer bison version via the Homebrew (brew utility) or any other open source package manager / repository. Some binaries for Mac OS X and Linux are available in the repository. 

##General description

FSMHSGen is a tool that generates all possible homing sequences for a Finite State Machine (FSM), which can be partial, nondeterministric and nonobservable. The algorithm for obtaining such homing sequences is based on the algorithm published in [1]. It must be noted that the term all possible, refers to the maximal amount of homing sequences depending on the configuration parameters. Indeed, the implementation uses a modified version of the algorithm presented in [1]; the truncating rule #2 has been extended not only to consider an upper bound on the length of the input sequence, but also, the time spent for deriving the homing sequences, and the memory utilized for such purpose. More details on the configuration parameters, and homing sequence generation algorithm can be found later in this document. 

The program takes as input a “.fsm” file with the FSM file format v1.0. The description of the FSM file format can be also found below in this document. It is important to note that the file extension is not required, the file can have any name and extension. Configuration parameters can be passed as command line parameters or via a configuration file. The configuration file and command line arguments are also found later in this document.

As an output, FSMHSGen provides to the standard output (console) a list of all possible homing sequences. Each homing sequence is represented as dot separated input list, and finishing with a newline (\n) for example, “HS:3.1.5\n”. Where the inputs correspond to the inputs in the “.fsm” input file.

FSMHSGen is also able to provide metrics regarding the complexity of the FSM specification. At the moment, FSMHSGen is able to compute the metrics specified below. The tool produces as an output the requested metrics. For example, “PSM = 0.1” if the metric of the partiality of the specification is requested on an FSM such that 10% of its transitions are undefined.

###Specification Complexity Metrics
Under the assumption that the system specification is given as a Finite State Machine (FSM) [3], i.e., a system that moves from one state to another when an input is applied and an output is produced. An FSM can be defined over a set of transitions in the following form: (current\_state, input, output, next\_state). The proposed metrics of complexity reflect the complexity of deriving the corresponding test suites, in order to test the quality of a given software. We propose the following list of metrics. 

####Partiality of the Specification

The percentage of transitions such that, at a given state (current\_state) no transitions are defined under a given input. The metric can be calculated as follows:

PSM=(|undef\_trans|)/(|total\_trans|)

####Non-determinism Degree of the Specification

The percentage of transitions such that, at a given state (current\_state) under a certain input, more than one transition is defined. The metric can be calculated as follows:

NDSM=(|nondet\_trans|)/(|total\_trans|)

####Non-observability Degree of the Specification

The percentage of transitions such that, at a given state (current\_state) under the same input/output pair the next state (next\_state) is different. The metric can be calculated as follows:

NDSM=(|nonobserv\_trans|)/(|total\_trans|)

####Length of Homing Sequences

The length of the state identification sequences significantly affects the total test length. We calculate the length of the shortest homing (identifies the final state) sequences of the FSM, i.e.:

HSLM=min⁡|HS\_1|,|HS\_2|,...)

##Configuration parameters of FSMHSGen
The configuration parameters of FSMHSGen affect the behavior in the execution of the tool. Mostly, they control the conditions to stop searching for the homing sequences and the debugging/logging information. There are two ways to specify the configuration parameters of FSMHSGen. The first is by supplying a configuration file to the executable file though its command line parameters with the option “-cf”. The second is to specify each parameter with its own command line parameter flag. The configuration file should specify a list of parameter names and the desired value in the format “<ParamName>=<value>”. If a parameter is repeated, the last value encountered will be taken, including the parameters passed in the command line. In a configuration file, after a # sign until the end of the line, it is considered as a comment. The parameter names, flags, and accepted values are listed below:

The parameter LogType specifies which logging style the tool should use. The possible value options are: “local” or “syslog”. The “local” value uses a file named FSMHSGen.log stored in the working directory. If the value is syslog, the program will use the local standard syslog facility [2] in the operating system, on the local7 facility. The default value (if log type is not specified) is set to local. Any other value will produce a warning. The associated command line flag is “-lt” 

###LogLevel
The parameter LogLevel specifies the verbosity on the logged output, as specified in [2], these values are:
0	Emergency: system is unusable
1	Alert: action must be taken immediately
2	Critical: critical conditions
3	Error: error conditions
4	Warning: warning conditions
5	Notice: normal but significant condition
6	Informational: informational messages
7	Debug: debug-level messages 
The default value for the LogLevel parameter is 3(error). Therefore, only messages which are more important (less in numeric value) or equally as important as error will be logged. In order to see debug info, for instance, the configuration must choose the value of 7. . Any value other than the listed before will produce a warning. The associated command line flag is “-ll”

###MaxMemSize
The parameter MaxMemSize specifies the maximal amount of RAM memory (in MB) the tool can utilize. When the homing sequence algorithm reaches the value of RAM specified by the MaxMemSize parameter, a notice is produced and the tool terminates its execution. A value of 0 means unlimited. The default value is 0. Negative integers or other inputs will be ignored and a warning will be produced. The associated command line flag is “-mm”

###MaxTime
The parameter MaxTime specifies the maximal amount of time (in seconds) the generation of homing sequences can take. When the homing sequence algorithm reaches the time specified by the MaxTime parameter, a notice is produced and the tool terminates its execution. A value of 0 means unlimited. The default value is 0. Negative integers or other inputs will be ignored and a warning will be produced. The associated command line flag is “-mt” 

###MaxHSLength
The parameter MaxHSLength specifies the maximal length of the potential homing sequences. This parameter, as other maximum limits, when the sequences under investigation reach the value of MaxHSLength, a notice is produced and the program terminates its execution. A value of 0 means there is no limit for the length of the homing sequences. The default value of MaxHSLength is 0. The associated command line flag is “-ml”.

###HSLM
The parameter HSLM specifies the need to compute and report the metric of the minimal length of a homing sequence of the given FSM specification. The possible values for this parameter are true and false. The default value is false. The associated command line flag is “-hslm”. Note that this flag takes no value afterwards, its presence implies HSLM=true.

###PSM
The parameter PSM specifies the need to compute and report the metric of the degree of partiality of the given FSM specification. The possible values for this parameter are true and false. The default value is false. The associated command line flag is “-psm”. Note that this flag takes no value afterwards, its presence implies PSM=true.

###NDSM
The parameter NDSM specifies the need to compute and report the metric of the degree of nondterminism of the given FSM specification. The possible values for this parameter are true and false. The default value is false. The associated command line flag is “-ndsm”. Note that this flag takes no value afterwards, its presence implies NDSM=true.

###NOSM
The parameter NOSM specifies the need to compute and report the metric of the degree of nonobservability of the given FSM specification. The possible values for this parameter are true and false. The default value is false. The associated command line flag is “-nosm”. Note that this flag takes no value afterwards, its presence implies NOSM=true.

###Execution example: 
Assume the tool is called with the following parameters: 
_FSMHSGen -cf fsmhsgen.conf -ml 4 inputFSM.fsm_

And the fsmhsgen.conf file contains:
\##################################

\## Simple FSMHSGen configuration file ##

\##################################

\# Maximal amount of memory to use

MaxMemSize=0 #(unlimited)

\# Maximal time allowed to execute the experiment, 10s

MaxTime=10

\# Maximal HS length

MaxHSLength=0

\# Type of logging

LogType=syslog # local syslog 

\# Log level

LogLevel=7 # debug, log all

\# hslm \# ndsm \# psm \# nosm

hslm=true

ndsm=true

psm=true

nosm=true

With the previous configuration parameters, the tool will run until the input sequences under evaluation reach the length of 4 or 10 seconds have passed, it will also compute all the complexity of the specification metrics. Note that the configuration file parameter is overwritten by the command line parameter since it appears later. Finally, note that the configuration file is case insensitive, but the command line parameters are case sensitive.

###FSM file format

The FSM file format is a very simple format used to describe an FSM. An FSM M is formally a tuple (S, I, O, hs, S’), where S is a finite nonempty set of states with a nonempty subset S' of initial states; I and O are finite input and output alphabets; hs is a transition relation in the Cartesian product SxIxOxS. In order to describe such object, the “.fsm” file format is composed of two main parts. The first part contains the information about the proper sets of the FSM. It assumes all labels of the set are numbered in order, and starting from 0. For example, the header: “S 4 I 3 O 2” defines an FSM with 4 states (labeled by “0”, “1”, “2”, “3”), three inputs (labeled by “0”, “1”, “2”), and two outputs (labeled by “0”, “1”). The second part of the file format contains the listing of the elements of the transition relation, in the format s i s’ o, where s is the current state, i is the input, s’ is the next state, and o is the output. For example, “0 2 3 1” defines a transition from state “0”, with input “2”, output “1” to state “3”. Finally, coments in the FSM file format are allowed after the sequence “//” and until the end of the line. Following is a list of header tags, and their allowed values.



####F

F must be at the beginning of the FSM file, it defines the type of FSM. Our software does not take into consideration the value of F, however, certain tools might rely on this option to optimize the treatment of the machine. At the moment, the tool parses any number following the F specification, example: F 3. 



####s

s defines the number of states of the FSM. This tag should be followed by a number, example S 4.



####i

i defines the number of inputs of the FSM. This tag should be followed by a number, example I 4.



####o

o defines the number of output of the FSM. This tag should be followed by a number, example O 3.



####n0

n0 defines the initial state of the FSM. This tag should be followed by a number, example n0 3.



####p

p defines the total number of transitions. This tag should be followed by a number, example p 13, this implies the transition relation has 13 elements. In order to describe each of the elements (or transitions) is explained below.



####Transitions

As explained before, each transition is composed by four numbers, representing the current state, the input, the output, and the next state respectfully. As an example, “0 2 3 1” defines a transition from state “0”, with input “2”, output “1” to state “3”.

An example FSM file is given below:

F 3 

s 3

i 3

o 2

n0 0

p 10 

//transitions start here 

0 1 1 1

0 2 2 0

1 0 2 0

1 1 1 0

1 1 0 1

1 2 1 0

2 0 2 0

2 1 2 0

2 0 1 0

2 2 1 1

The corresponding FSM is shown in the following figure.

![Example FSM](/images/ExFSM.jpg)

As an example, the output of the program assuming execution example, and the input file the previously described FSM, the program produces the following output:
_FSMHSGen -cf fsmhsgen.conf -ml 4 test.fsm

HS: 2.2

HS: 2.0.2

HS: 2.1.2

HS: 2.2.2

HS: 2.2.1

HS: 2.2.0

HS: 1.2.2

HS: 1.2.0.2

HS: 1.2.1.2

HS: 1.2.2.2

HS: 1.2.2.1

HS: 1.2.2.0

HS: 1.1.2.2

HS: 2.2.0.2

HS: 2.2.0.1

HS: 2.2.1.2

HS: 2.2.1.1

HS: 2.2.2.2

HS: 2.2.2.1

HS: 2.2.2.0

HS: 2.1.1.2

HS: 2.1.2.2

HS: 2.1.2.1

HS: 2.0.0.2

HS: 2.0.1.2

HS: 2.0.2.2

HS: 2.0.2.1

HS: 2.0.2.0

PSM = 0.1 (1 / 10)

NDSM = 0.4 (4 / 10)

NOSM = 0.1 (1 / 10)

HSLM = 2
_

##References

[1] Natalia Kushik, Jorge López, Ana Cavalli, Nina Yevtushenko: “Improving Protocol Passive Testing through ‘Gedanken’ Experiments with Finite State Machines”,  in the proceedings of the IEEE International Conference on Software Quality, Reliability and Security, QRS, Vienna, Austria, 2016.


[2] Rainer Gerhards: “The Syslog protocol”, Request for Comments (RFC) 5424, DOI: 10.17487/rfc5424, 2015.





