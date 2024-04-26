# fine-tuning linux network's configurable runtime parameters
sudo sysctl net.core.netdev_budget=600

cat /proc/sys/net/core/netdev_budget

sudo sysctl net.core.netdev_max_backlog=50000

cat /proc/sys/net/core/netdev_max_backlog

sudo sysctl net.core.rmem_default=425984

cat /proc/sys/net/core/rmem_default

sudo sysctl net.core.rmem_max=425984

cat /proc/sys/net/core/rmem_max

sudo sysctl net.core.wmem_default=212992

cat /proc/sys/net/core/wmem_default

sudo ethtool -K ens5 gro on

sudo ethtool --set-ring ens5 rx 8192 tx 512

sudo ip link set dev ens5 mtu 9001

sudo sysctl -w net.ipv4.tcp_rmem='4096 425984 425984'

sudo sysctl -w net.ipv4.tcp_wmem='8192 425984 425984'
