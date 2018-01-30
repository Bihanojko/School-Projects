from PIL import Image, ImageDraw
import framework.utils as utils
import argparse
import math
import os
import numpy

# auxiliary sgn function (signul(0) = 1)
signum = lambda x: -1 if x < 0 else 1

# definition of neighbouring cells
cells = [(-1, -1), (-1, 0), (-1, 1), (0, 1), (1, 1), (1, 0), (1, -1), (0, -1), (-1, -1)]

#
# Repurposed code from rtshadow - calculate poincare index
#
def get_angle(left, right):
	angle = left - right
	if abs(angle) > 180:
		angle = -1 * signum(angle) * (360 - abs(angle))
	return angle

def poincare_index_at(i, j, angles, tolerance):
	deg_angles = [math.degrees(angles[i - k][j - l]) % 180 for k, l in cells]
	index = 0
	for k in range(0, 8):
		if abs(get_angle(deg_angles[k], deg_angles[k + 1])) > 90:
			deg_angles[k + 1] += 180
		index += get_angle(deg_angles[k], deg_angles[k + 1])

	if 180 - tolerance <= index and index <= 180 + tolerance:
		return "loop"
	if -180 - tolerance <= index and index <= -180 + tolerance:
		return "delta"
	if 360 - tolerance <= index and index <= 360 + tolerance:
		return "whorl"
	return "none"
#
# End of repurposed code
#

# Find singularities in image
def calculate_singularities(im, angles, tolerance, W):
	result = im.convert("RGB")

	draw = ImageDraw.Draw(result)
	colors = {"loop" : (150, 0, 0), "delta" : (0, 150, 0), "whorl": (0, 0, 150)}

	avg = average_color(im)
	loops = []
	deltas = []
	whorls = []

	# for each bloxk of size WxW calculate Poincare index (except on edges)
	for i in range(1, len(angles) - 1):
		for j in range(1, len(angles[i]) - 1):
			singularity = poincare_index_at(i, j, angles, tolerance)
			if singularity != "none" and valid_singularity(i, j, len(angles), len(angles[i]), im, W, avg):
				position = [i, j]
				if not duplicate_singularity(loops, deltas, whorls, singularity, position):
					add_singularity(loops, deltas, whorls, singularity, position)
					draw.ellipse([(i * W + W / 2, j * W + W / 2), ((i + 1) * W  + W / 2, (j + 1) * W + W / 2)], outline = colors[singularity])

	del draw

	return [calculate_type(loops, deltas, whorls), result]

# calculate fingerprint class from found singularities
def calculate_type(loops, deltas, whorls):
	margin = 2
	result = {}
	result["type"] = "unknown"
	result["certainty"] = 0

	# whorl found
	if len(whorls) == 1:
		result["type"] = "whorl"
		result["certainty"] = 1

	# two loops and delstas found
	if len(loops) == 2:
		result["type"] = "twin loop"
		if len(deltas) == 2:
			result["certainty"] = 1
		else:
			result["certainty"] = 0.75

	# no singularity found
	if len(loops) == 0:
		result["type"] = "arch"
		if len(deltas) == 0:
			result["certainty"] = 1
		else:
			result["certainty"] = 0.5

	# one loop and delta found, comapre relative position
	if len(loops) == 1 and len(deltas) == 1:
		if loops[0][0] >= deltas[0][0] + margin:
			result["type"] = "right loop"
			result["certainty"] = 1
		elif loops[0][0] <= deltas[0][0] - margin:
			result["type"] = "left loop"
			result["certainty"] = 1
		else:
			result["type"] = "tended arch"
			result["certainty"] = 1

	return result

# calculate average intensity of pixels across image
def average_color(im):
	sum = 0;
	data = numpy.array(im)

	for i in range(0, im.size[1]):
		for j in range(0, im.size[0]):
			sum += data[i, j]

	return sum / data.size


# check if found singularity is not on the edge of fingerprint or image
def valid_singularity(i, j, wi, wj, im, W, avg):
	return not edge_of_image(i, j, wi, wj) and not edge_of_fingerprint(i, j, im, W, avg)


# check if found singularity is on the edge of the image
def edge_of_image(i, j, wi, wj):
	i = i if i < wi - i else wi - i
	j = j if i < wj - j else wj - j

	return False if (i < wi - 4 and i > 4) and (j < wj - 4 and j > 4) else True


# check if found singularity is on the edge of the fingerprint
def edge_of_fingerprint(i, j, im, W, avg):
	data = numpy.array(im)

	# check all neighbouring patches
	for offset in cells:
		sum = 0
		for k in range(0, W):
			for l in range(0, W):
				x = (i + offset[0]) * W + k
				y = (j + offset[1]) * W + l
				x = x if x < im.size[1] else im.size[1] - 1
				y = y if y < im.size[0] else im.size[0] - 1
				sum += data[x, y]
		if sum / (W * W) > avg * 1.5:
			#on the edge
			return True

	return False #not on the edge

# check if found singularity is duplicate of previously found singularity
def duplicate_singularity(loops, deltas, whorls, singularity, position):
	if singularity == "loop":
		array = loops
	if singularity == "delta":
		array = deltas
	if singularity == "whorl":
		array = whorls

	return check_for_duplicate(array, position)

# check if singularity with similar position is already in list
def check_for_duplicate(array, position):
	up = [position[0] - 1, position[1]]
	left = [position[0], position[1] - 1]
	diag = [position[0] - 1, position[1] - 1]
	for item in array:
		if numpy.array_equal(item, up) or numpy.array_equal(item, left) or numpy.array_equal(item, diag):
			return True
	return False 

# add singularity to result list
def add_singularity(loops, deltas, whorls, singularity, position):
	if singularity == "loop":
		loops.append(position)
	if singularity == "delta":
		deltas.append(position)
	if singularity == "whorl":
		whorls.append(position)
	return


# parse arguments
parser = argparse.ArgumentParser(description="Singularities with Poincare index")
parser.add_argument("image", nargs=1, help = "Path to image")
parser.add_argument("block_size", nargs=1, help = "Block size")
parser.add_argument("tolerance", nargs=1, help = "Tolerance for Poincare index")
args = parser.parse_args()

# load image
im = Image.open(args.image[0])

# convert to grayscale
im = im.convert("L")

# set block size
W = int(args.block_size[0])



#
# Inicialization of utility functions rtshadow uses for singularity finding
#
f = lambda x, y: 2 * x * y
g = lambda x, y: x ** 2 - y ** 2

angles = utils.calculate_angles(im, W, f, g)
angles = utils.smooth_angles(angles)
#
# End of inicialization
#

# Calculate type, print result and display found singularities
result = calculate_singularities(im, angles, int(args.tolerance[0]), W)
print result[0]
result[1].show()
