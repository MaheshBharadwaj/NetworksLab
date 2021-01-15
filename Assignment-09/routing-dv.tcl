set ns [new Simulator]

$ns color 1 red
$ns color 2 blue

set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]
set n6 [$ns node]
set n7 [$ns node]
set n8 [$ns node]
set n9 [$ns node]
set n10 [$ns node]
set n11 [$ns node]

set nr [open out.tr w]
$ns trace-all $nr
set nf [open out.nam w]
$ns namtrace-all $nf

proc finish { } {
global ns nr nf
$ns flush-trace
close $nf
close $nr
exec nam out.nam &
exit 0
}

# $ns duplex-link $n0 $n1 1Mb 10ms DropTail
# $ns duplex-link $n1 $n2 1Mb 10ms DropTail
# $ns duplex-link $n2 $n3 1Mb 10ms DropTail
# $ns duplex-link $n3 $n4 1Mb 10ms DropTail
# $ns duplex-link $n4 $n5 1Mb 10ms DropTail
# $ns duplex-link $n5 $n6 1Mb 10ms DropTail
$ns duplex-link $n6 $n7 1Mb 10ms DropTail
# $ns duplex-link $n7 $n8 1Mb 10ms DropTail

$ns duplex-link $n0 $n8 1Mb 10ms DropTail
$ns duplex-link $n1 $n10 1Mb 10ms DropTail
$ns duplex-link $n0 $n9 1Mb 10ms DropTail
$ns duplex-link $n9 $n11 1Mb 10ms DropTail
$ns duplex-link $n10 $n11 1Mb 10ms DropTail
$ns duplex-link $n11 $n5 1Mb 10ms DropTail


# $ns duplex-link-op $n0 $n1 orient right
# $ns duplex-link-op $n1 $n2 orient down
# $ns duplex-link-op $n2 $n3 orient down
# $ns duplex-link-op $n3 $n4 orient left
# $ns duplex-link-op $n4 $n5 orient left
# $ns duplex-link-op $n5 $n6 orient left
$ns duplex-link-op $n6 $n7 orient left
# $ns duplex-link-op $n7 $n8 orient up
$ns duplex-link-op $n8 $n0 orient up
$ns duplex-link-op $n5 $n11 orient up
$ns duplex-link-op $n0 $n9 orient right-down
$ns duplex-link-op $n9 $n11 orient right
$ns duplex-link-op $n11 $n10 orient right
$ns duplex-link-op $n1 $n10 orient left-down

set udp0 [new Agent/UDP]
$ns attach-agent $n0 $udp0
$udp0 set fid_ 1

set cbr0 [new Application/Traffic/CBR]
$cbr0 set packetSize_ 1000
$cbr0 set rate_ 0.15mb
$cbr0 set random_ true
$cbr0 attach-agent $udp0

set null0 [new Agent/Null]
$ns attach-agent $n5 $null0
$ns connect $udp0 $null0

set udp1 [new Agent/UDP]
$ns attach-agent $n1 $udp1
$udp1 set fid_ 2

set cbr1 [new Application/Traffic/CBR]
$cbr1 set packetSize_ 1000
$cbr1 set rate_ 0.10mb
$cbr1 set random_ true
$cbr1 attach-agent $udp1

set null1 [new Agent/Null]
$ns attach-agent $n5 $null1
$ns connect $udp1 $null1

$ns rtproto DV
$ns rtmodel-at 3.0 down $n11 $n5
$ns rtmodel-at 2.0 down $n7 $n6
$ns rtmodel-at 4.0 up $n11 $n5
$ns rtmodel-at 3.0 up $n7 $n6
$ns at 0.0 "$cbr0 start"
$ns at 0.75 "$cbr1 start"
$ns at 5.0 "finish"
$ns run

