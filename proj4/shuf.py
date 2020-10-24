#!/usr/bin/python
"""
Output lines randomly permutated from a file

Bonnie Liu
"""

import random, sys, argparse

#read from file
class randline:
	def __init__(self, filename):
		f = open(filename, 'r')
		self.entries = []
		for entry in f:
			self.entries.append(entry)
		f.close()
	def shuffle_lines(self):
		return random.sample(self.entries, len(self.entries))

#generate random numbers based on input range (-i)
class rand_for_input_range:
	def __init__(self, min_num, max_num):
		self.nums=[]
		for i in range(min_num, max_num+1):
			self.nums.append(i)
	def shuffle_nums(self):	
		random.shuffle(self.nums)
		self.shuffled_list = []
		for i in self.nums:
			self.shuffled_list.append(str(i))
		return self.shuffled_list

#print shuffled list
def final_print(lst, args):
	iteration_number = 0
	while args.repeat is True:
		if args.head_count is not None and iteration_number == int(args.head_count):
			sys.exit(0)
		if len(lst)!=0:
			index = random.randint(0, len(lst)-1)
			print(lst[index].rstrip())
		iteration_number += 1
	for i in range(len(lst)):
		if (args.head_count is not None) and (i == int(args.head_count)):
			return
		print(lst[i].rstrip())

def main():
	#add arguments
	usage_msg = """%%prog [OPTION]... FILE

Print a random permutation of the lines inputted.

If no file is provided or file name is '-', read from standard input."""
	parser = argparse.ArgumentParser(usage=usage_msg)
	parser.add_argument("-i", "--input-range", help="treat each number LO through HI as an input line")
	parser.add_argument("-n", "--head-count", help="output at most COUNT lines")
	parser.add_argument("-r", "--repeat", action="store_true", default=False, help="output lines can be repeated")
	parser.add_argument(dest='filename', nargs='?', help=argparse.SUPPRESS)
	args = parser.parse_args()
#	print(args)
	printme = []
	if (args.head_count is not None) and (int(args.head_count) == 0) and args.repeat is False:
		sys.exit(0)
	#if input range (-i)
	if args.input_range is not None:
		split_range = args.input_range.split('-')
		min_num = int(split_range[0])
		max_num = int(split_range[1])
		#potential errors
		if max_num < min_num or min_num < 0:
			parser.error("shuf: invalid input range ‘{}-{}’".format(split_range[0], split_range[1]))
		generator = rand_for_input_range(min_num, max_num)
		printme = generator.shuffle_nums()
	#if no file or (-) given
	elif (args.filename is None or args.filename == '-'):
		user_input = []
		for line in sys.stdin:
			user_input.append(line)
		printme = random.sample(user_input, len(user_input))
	else:
		generator = randline(args.filename)
		printme = generator.shuffle_lines()
	final_print(printme, args)

if __name__ == "__main__":
	main()
