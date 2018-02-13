#!/usr/bin/env python2
# upload using ArduinoISP
import argparse
import os
import time

def update(args):
	os.system('platformio run -d ArduinoISP/{} --target upload'.format(args.target))

def upload(args):
	os.system('platformio run -t program -d {}'.format(args.code))
	# os.system('platformio run -t program -d {}'.format(args.code))
	
if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('target', type=str, help="MCU to program.")
	parser.add_argument('code', type=str, help="code to upload.")
	args = parser.parse_args()
	update(args)
	upload(args)