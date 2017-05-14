#!/bin/sh
#Настройка netns для измерений.

#Создаем два виртуальных сетевых стека:

ip netns add NetTestServer
ip netns add NetTestClient

#Создаем 2 пары veth-интерфейсов, попарно связанных между собой линками

ip link add veth0 type veth peer name nteth0
ip link add veth1 type veth peer name nteth1

#По одному из попарных интерфейсов перемещаем в стеки NetTestServer и NetTestClient

ip link set nteth0 netns NetTestServer
ip link set nteth1 netns NetTestClient

#Поднимаем все созданные интерфейсы

ifconfig veth0 192.168.2.1/24 up
ip netns exec NetTestServer ifconfig nteth0 192.168.2.2/24 up

ifconfig veth1 192.168.3.1/24 up
ip netns exec NetTestClient ifconfig nteth1 192.168.3.2/24 up


