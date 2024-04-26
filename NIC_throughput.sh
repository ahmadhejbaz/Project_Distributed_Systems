#!/bin/bash

INTERVAL=1

while true
do
        Rbefore=`cat /sys/class/net/ens5/statistics/rx_bytes`
        Tbefore=`cat /sys/class/net/ens5/statistics/tx_bytes`
        sleep $INTERVAL

        Rafter=`cat /sys/class/net/ens5/statistics/rx_bytes`
        Tafter=`cat /sys/class/net/ens5/statistics/tx_bytes`

        result_T=`expr $Tafter - $Tbefore`
        result_R=`expr $Rafter - $Rbefore`

        echo -e "TX $result_T\tbytes/s RX $result_R\tbytes/s"
done
