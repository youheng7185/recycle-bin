#!/bin/bash
pyocd erase --chip -t stm32h723xx
pyocd flash -t stm32h723xx build/recycle-bin-main.bin
