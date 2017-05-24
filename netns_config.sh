#!/bin/sh

#Предварительно требуется включить маршрутизацию на этом и на уделенном хосте:
# sudo su
# echo 1 > /proc/sys/net/ipv4/ip_forward

# На удаленном хосте требуется настроить маршрут "на себя", т.е. весь принятый трафик должен заворачиваться обратно на тот же интерфейс:
# ip route add default via <ip>

EMUL=0
REMOTE_IP=192.168.4.2
while getopts ei:a: option
do
	case "${option}"
	in
	e) EMUL=1
		shift;;
	i) NI=${OPTARG}
		shift; shift;;
	a) REMOTE_IP=${OPTARG}
		shift; shift;;
	esac
done

if [ $EMUL -eq 1 ]; then
	NI=veth2
fi

case $1 in
'config')

	#Настройка netns для измерений.

	#Создаем два виртуальных сетевых стека:

	ip netns add NetTestServer
	ip netns add NetTestClient
	echo "Созданы виртуальные стеки"
	
	# Создаем эмулятор удаленного хоста
	if [ $EMUL -eq 1 ]; then
		ip netns add RemoteStub
		echo "Создан эмулятор удаленного хоста"
	fi

	#Создаем пары veth-интерфейсов, попарно связанных между собой линками

	ip link add veth0 type veth peer name nteth0
	ip link add veth1 type veth peer name nteth1
	ip link set nteth0 netns NetTestServer
	ip link set nteth1 netns NetTestClient
	echo "Созданы veth-интерфейсы к виртуальным стекам"

	if [ $EMUL -eq 1 ]; then
		ip link add veth2 type veth peer name nteth2
		ip link set nteth2 netns RemoteStub
		echo "Создан veth-интерфейс к эмулятору удаленного хоста"
	fi
	
	#Поднимаем все созданные интерфейсы

	ifconfig veth0 192.168.2.1/24 up
	ip netns exec NetTestServer ifconfig nteth0 192.168.2.2/24 up
	ifconfig veth1 192.168.3.1/24 up
	ip netns exec NetTestClient ifconfig nteth1 192.168.3.2/24 up
	echo "veth-интерфейсы к виртуальным стекам включены"

	if [ $EMUL -eq 1 ]; then
		ifconfig veth2 192.168.4.1/24 up
		ip netns exec RemoteStub ifconfig nteth2 192.168.4.2/24 up
		echo "veth-интерфейс к эмулятору удаленного хоста включен"
	fi

	#Прописываем маршруты из стеков NetTestServer и NetTestClient
	ip netns exec NetTestServer ip route add default via 192.168.2.1
	ip netns exec NetTestClient ip route add default via 192.168.3.1
	echo "Созданы маршруты для виртуальных стеков"
	#Отключаем icmp request для предотвращения сообщений Protocol unreachable
	ip netns exec NetTestClient iptables -A OUTPUT -p icmp --icmp-type 3 -j DROP
	echo "Отключены ответы icmp type 3"

	if [ $EMUL -eq 1 ]; then
		echo 1 > /proc/sys/net/ipv4/ip_forward
		ip netns exec RemoteStub ip route add default via 192.168.4.1
		echo "Включено перенаправление и созданы маршруты в эмуляторе"
	fi

	#Прописываем маршруты для перенаправления потоков из NetTestServer и NetTestClient во внешнюю сеть или в эмулятор
	ip route add default via 192.168.2.2 table 1
	ip route add default via 192.168.3.2 table 2
	ip route add default via $REMOTE_IP table 3
	ip rule add from 192.168.3.2 to 192.168.2.2 iif $NI lookup 1
	ip rule add from 192.168.2.2 to 192.168.3.2 iif $NI lookup 2
	ip rule add from 192.168.3.2 to 192.168.2.2 iif veth1 lookup 3
	ip rule add from 192.168.2.2 to 192.168.3.2 iif veth0 lookup 3
	echo "Созданы маршруты для реального/эмулированного удаленного хоста"
	
	;;

'reset')
	ip rule del from 192.168.3.2
	ip rule del from 192.168.3.2
	ip rule del from 192.168.2.2
	ip rule del from 192.168.2.2
	ip route flush table 1
	ip route flush table 2
	ip route flush table 3
	echo "Удалены маршруты для реального/эмулированного удаленного хоста"
	ip netns del NetTestServer
	ip netns del NetTestClient
	ip netns del RemoteStub
	echo "Удалены виртуальные стеки"
	;;

*)
	echo "Использование: $0 [ -e ] [ -i <интерфейс> ] [ -a <адрес хоста> ] [ config | reset ]"
	echo "\t[ -e ] - эмуляция удаленного хоста на локальном"
	echo "\t[ -i <интерфейс> ] - имя интерфейса (при наличии опции -e не указывается), указывать обязательно при выполнении команды config"
	echo "\t[ -a <адрес хоста> ] - адрес удаленного или эмулированного хоста"
	echo "\t[ config | reset ] - выполняемая команда, config - сконфигурировать сетевые стеки, reset - сбросить конфигурацию"

esac

