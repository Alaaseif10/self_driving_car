import urllib.request
import cv2
import numpy as np 
import math
import os
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np
import cv2
import PIL 
from firebase import firebase
import numpy as np
import cv2
import urllib.request
from firebase import firebase
import matplotlib.pyplot as plt



firebase = firebase.FirebaseApplication("https://cartask-2ee8b-default-rtdb.firebaseio.com/", None)


def grayscale(img):
    return cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)

def gaussian_blur(img, kernel_size):
    return cv2.GaussianBlur(img, (kernel_size, kernel_size), 0)

def canny(img, low_threshold, high_threshold):
    return cv2.Canny(img, low_threshold, high_threshold)

def region_of_interest(img, vertices):
    mask = np.zeros_like(img)
    if len(img.shape) > 2:
        channel_count = img.shape[2]
        ignore_mask_color = (255,) * channel_count
    else:
        ignore_mask_color = 255
    cv2.fillPoly(mask, vertices, ignore_mask_color)
    masked_image = cv2.bitwise_and(img, mask)
    return masked_image

def draw_lines(img, lines, color=[0, 0, 255], thickness=2):
    for line in lines:
        for x1, y1, x2, y2 in line:
            cv2.line(img, (x1, y1), (x2, y2), color, thickness)
def display_lines(frame, lines, line_color=(0, 255, 0), line_width=5):
    line_image = np.zeros_like(frame)
    if lines is not None:
        for line in lines:
            cv2.line(line_image, (line[0], line[1]), (line[2], line[3]), line_color, line_width)
    line_image = cv2.addWeighted(frame, 0.8, line_image, 1, 1)
    return line_image

def hough_lines(img, rho, theta, threshold, min_line_len, max_line_gap):
    lines = cv2.HoughLinesP(img, rho, theta, threshold, np.array([]), minLineLength=min_line_len,maxLineGap=max_line_gap)
    line_img = np.zeros((img.shape[0], img.shape[1], 3), dtype=np.uint8)
    draw_lines(line_img, lines)
    return line_img, lines

def lane_finder(image):
    #gray_img = grayscale(image)
    # blur_gray = gaussian_blur(image, 5)
    edges = canny(image, 200, 400)
    imshape = image.shape
    vertices = np.array([[(0, imshape[0]), (410, 150), (490, 215), (imshape[1], imshape[0])]], dtype=np.int32)
    masked_edges = region_of_interest(edges, vertices)
    lane_img,lines = hough_lines(masked_edges, 1, np.pi / 180, 15, 20, 8)
    lines = average_slope_intercept(lane_img, lines)
    return lines

def make_points(frame, line):
    height, width, _ = frame.shape
    slope, intercept = line
    y1 = height  # bottom of the frame
    y2 = int(y1 * 0.5)  # make points from middle of the frame down

    x1 = max(-width, min(2 * width, int((y1 - intercept) / slope)))
    x2 = max(-width, min(2 * width, int((y2 - intercept) / slope)))
    return [x1, y1, x2, y2]

def average_slope_intercept(frame, line_segments):
    
    lane_lines = []
    if line_segments is None:
        return lane_lines

    height, width, _ = frame.shape
    left_fit = []
    right_fit = []

    boundary = 1/3
    left_region_boundary = width * (1 - boundary)  # left lane line segment should be on left 2/3 of the screen
    right_region_boundary = width * boundary # right lane line segment should be on left 2/3 of the screen

    for line_segment in line_segments:
        for x1, y1, x2, y2 in line_segment:
            fit = np.polyfit((x1, x2), (y1, y2), 1)
            slope = fit[0]
            intercept = fit[1]
            if slope < 0:
                if x1 < left_region_boundary and x2 < left_region_boundary:
                    left_fit.append((slope, intercept))
            else:
                if x1 > right_region_boundary and x2 > right_region_boundary:
                    right_fit.append((slope, intercept))

    left_fit_average = np.average(left_fit, axis=0)
    if len(left_fit) > 0:
        lane_lines.append(make_points(frame, left_fit_average))

    right_fit_average = np.average(right_fit, axis=0)
    if len(right_fit) > 0:
        lane_lines.append(make_points(frame, right_fit_average))
    
    return lane_lines
def steering_angle(frame, lines):
    
    height, width, _ = frame.shape
    if len(lines) == 2:
        mid = int(width / 2)
        x_offset = (lines[0][2] + lines[1][2]) / 2 - mid
        y_offset = int(height / 2)
    else:
        x_offset = lines[0][2] - lines[0][0]
        y_offset = int(height / 2)

    angle_radian = math.atan(x_offset / y_offset)
    angle_deg = int(angle_radian * 180.0 / math.pi)
    angle_deg += 90
    return angle_deg

while True:
    try:
        url ='http://172.28.162.30:8080/shot.jpg?1'
        img=urllib.request.urlopen(url)
        im=np.array(bytearray(img.read()),dtype=np.uint8)
        img0=cv2.imdecode(im,-1)


        # img0 = cv2.imread("c (11).jpeg")
        hsv_frame = cv2.cvtColor(img0,cv2.COLOR_BGR2HSV)
        thresholded_img = cv2.inRange(hsv_frame,np.array([60,20,20]),np.array([150,255,255]))
        lines = lane_finder(thresholded_img)
        # print(len(lines))
        angle = steering_angle(img0,lines)
        # print(angle)

        # x = display_lines(img0,lines)
        # cv2.imshow("x",x)
        # cv2.waitKey(1)

        # # print(img0.shape)
        # plt.imshow(x)


        if (angle <90):
            firebase.put('','status/status', 1)
            print(1)
        # elif (positive == 0 and negative != 0):
        #     firebase.put('','status/status', 2)
        # elif (positive != 0 and negative == 0):
        #     firebase.put('','status/status', 3)
        else:
            firebase.put('','status/status', 4)
            print(4)
            
        firebase.put('','status/angle', angle)
        # print(angle)

            
    except :
        pass