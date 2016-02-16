#!/usr/bin/python

# This simple program was proposed here: http://stackoverflow.com/a/26044115/486568

import cv2
import numpy as np
import sys

def nothing(x):
    pass

def main(argv):
    
    if len(argv) <= 0:
        sys.exit( "The first argument should be the camera number. Ex. test@pc:~$ python hsv_filter.py 1" )
    
    cap = cv2.VideoCapture(int(argv[0]))

    # Creating a window for later use
    cv2.namedWindow('result')

    # Starting with 100's to prevent error while masking
    h,s,v = 100,100,100

    # Creating track bar
    cv2.createTrackbar('h', 'result',0,179,nothing)
    cv2.createTrackbar('s', 'result',0,255,nothing)
    cv2.createTrackbar('v', 'result',0,255,nothing)

    while(1):
        _, frame = cap.read()

        #converting to HSV
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        # get info from track bar and appy to result
        h = cv2.getTrackbarPos('h','result')
        s = cv2.getTrackbarPos('s','result')
        v = cv2.getTrackbarPos('v','result')

        # Normal masking algorithm
        lower_blue = np.array([h,s,v])
        upper_blue = np.array([180,255,255])

        mask = cv2.inRange(hsv,lower_blue, upper_blue)

        result = cv2.bitwise_and(frame,frame,mask = mask)

        cv2.imshow('result',result)

        k = cv2.waitKey(5) & 0xFF
        # escape or 'q' to exit
        if k == 27 or k == 113:
            break

    cap.release()

    cv2.destroyAllWindows()


if __name__ == "__main__":
    main(sys.argv[1:])
    