set ns [new Simulator]
set tf [open lab9.tr w]
$ns trace-all $tf
set nf [open lab9.nam w]
$ns namtrace-all $nf

set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
$ns make-lan "$n0 $n1 $n2 $n3" 100Mb 100ms LL Queue/DropTail Mac/802_3
$ns color 1 "red"
$ns color 2 "blue"


set tcp0 [new Agent/TCP]
$ns attach-agent $n0 $tcp0
set ftp0 [new Application/FTP]
$ftp0 attach-agent $tcp0

set tcp1 [new Agent/TCP]
$ns attach-agent $n1 $tcp1
set ftp1 [new Application/FTP]
$ftp1 attach-agent $tcp1

set null1 [new Agent/Null]
$ns attach-agent $n1 $null1

set udp2 [new Agent/TCP]
$ns attach-agent $n2 $udp2
set cbr2 [new Application/Traffic/CBR]
$cbr2 attach-agent $udp2
set sink2 [new Agent/TCPSink]
$ns attach-agent $n2 $sink2

set sink3 [new Agent/TCPSink]
$ns attach-agent $n3 $sink3

#$tcp0 set packetSize_ 5000
$tcp0 set interval_ 0.001

#$tcp1 set packetSize_ 5000
$tcp1 set interval_ 0.001

#$cbr2 set packetSize_ 5000
$cbr2 set interval_ 0.001
$tcp0 set class_ 1
$tcp1 set class_ 2
$udp2 set class_ 3
$ns connect $tcp0 $sink2

$ns connect $tcp1 $sink3

$ns connect $udp2 $null1

proc finish {} {
global ns nf tf
$ns flush-trace
exec lab9.nam &
close $tf
close $nf
exit 0
}

$ns at 0.1 "$tcp0 start"
$ns at 0.5 "$tcp1 start"
$ns at 1.0 "$udp2 start"
$ns at 4.0 "finish"
$ns run
