*version 9.1 617452189
u 44
V? 2
U? 2
R? 4
D? 3
@libraries
@analysis
@targets
@attributes
@translators
a 0 u 13 0 0 0 hln 100 PCBOARDS=PCB
a 0 u 13 0 0 0 hln 100 PSPICE=PSPICE
a 0 u 13 0 0 0 hln 100 XILINX=XILINX
@setup
unconnectedPins 0
connectViaLabel 0
connectViaLocalLabels 0
NoStim4ExtIFPortsWarnings 1
AutoGenStim4ExtIFPorts 1
@index
pageloc 1 0 1965 
@status
c 119:03:25:19:31:58;1556213518
*page 1 0 970 720 iA
@ports
port 3 GND_ANALOG 470 130 h
port 17 GND_ANALOG 420 260 h
port 20 GND_ANALOG 330 270 h
port 41 GND_ANALOG 240 270 h
@parts
part 22 D1N4002 240 150 v
a 0 s 0:13 0 0 0 hln 100 PKGTYPE=DO-41
a 0 s 0:13 0 0 0 hln 100 GATE=
a 0 a 0:13 0 0 0 hln 100 PKGREF=D2
a 0 ap 9 0 17 4 hln 100 REFDES=D2
a 0 sp 11 0 13 57 hln 100 PART=D1N4002
part 2 VDC 470 90 h
a 0 sp 0 0 22 37 hln 100 PART=VDC
a 0 a 0:13 0 0 0 hln 100 PKGREF=V1
a 1 ap 9 0 24 7 hcn 100 REFDES=V1
part 19 POT 330 270 v
a 0 sp 11 0 20 25 hln 100 PART=POT
a 0 s 0:13 0 0 0 hln 100 PKGTYPE=
a 0 a 0:13 0 0 0 hln 100 PKGREF=R2
a 0 ap 9 0 15 0 hln 100 REFDES=R2
part 16 LM324 380 210 h
a 0 s 0:13 0 0 0 hln 100 PKGTYPE=DIP14
a 0 s 0:13 0 0 0 hln 100 GATE=A
a 0 a 0:13 0 0 0 hln 100 PKGREF=U1
a 0 ap 9 0 56 8 hcn 100 REFDES=U1A
part 40 r 240 270 v
a 0 sp 0 0 0 10 hlb 100 PART=r
a 0 s 0:13 0 0 0 hln 100 PKGTYPE=RC05
a 0 s 0:13 0 0 0 hln 100 GATE=
a 0 a 0:13 0 0 0 hln 100 PKGREF=R3
a 0 ap 9 0 15 0 hln 100 REFDES=R3
part 1 titleblk 970 720 h
a 1 s 13 0 350 10 hcn 100 PAGESIZE=A
a 1 s 13 0 180 60 hcn 100 PAGETITLE=
a 1 s 13 0 300 95 hrn 100 PAGENO=1
a 1 s 13 0 340 95 hrn 100 PAGECOUNT=1
@conn
w 33
s 350 250 380 250 32
w 25
s 240 90 240 120 8
s 470 90 420 90 6
s 420 90 420 200 13
s 420 90 330 90 12
s 330 90 240 90 31
s 330 90 330 230 34
w 24
s 380 210 240 210 36
s 240 210 240 150 38
s 240 210 240 230 42
@junction
j 470 130
+ s 3
+ p 2 -
j 420 260
+ s 17
+ p 16 V-
j 330 270
+ s 20
+ p 19 1
j 240 150
+ p 22 1
+ w 24
j 240 120
+ p 22 2
+ w 25
j 420 90
+ w 25
+ w 25
j 470 90
+ p 2 +
+ w 25
j 420 200
+ p 16 V+
+ w 25
j 330 90
+ w 25
+ w 25
j 350 250
+ p 19 t
+ w 33
j 380 250
+ p 16 -
+ w 33
j 330 230
+ p 19 2
+ w 25
j 380 210
+ p 16 +
+ w 24
j 240 270
+ s 41
+ p 40 1
j 240 230
+ p 40 2
+ w 24
j 240 210
+ w 24
+ w 24
@attributes
a 0 s 0:13 0 0 0 hln 100 PAGETITLE=
a 0 s 0:13 0 0 0 hln 100 PAGENO=1
a 0 s 0:13 0 0 0 hln 100 PAGESIZE=A
a 0 s 0:13 0 0 0 hln 100 PAGECOUNT=1
@graphics
