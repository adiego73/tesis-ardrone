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
    h_min,s_min,v_min = 100,100,100
    h_max,s_max,v_max = 180,255,255

    # Creating track bar
    cv2.createTrackbar('H max', 'result',0,180,nothing)
    cv2.createTrackbar('H min', 'result',0,180,nothing)
    cv2.createTrackbar('S max', 'result',0,255,nothing)
    cv2.createTrackbar('S min', 'result',0,255,nothing)
    cv2.createTrackbar('V max', 'result',0,255,nothing)
    cv2.createTrackbar('V min', 'result',0,255,nothing)

    while(1):
        _, frame = cap.read()

        #converting to HSV
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        # get info from track bar and appy to result
        h_min = cv2.getTrackbarPos('H min','result')
        s_min = cv2.getTrackbarPos('S min','result')
        v_min = cv2.getTrackbarPos('V min','result')
        
        h_max = cv2.getTrackbarPos('H max','result')
        s_max = cv2.getTrackbarPos('S max','result')
        v_max = cv2.getTrackbarPos('V max','result')

        # Normal masking algorithm
        lower = np.array([h_min,s_min,v_min])
        upper = np.array([h_max,s_max,v_max])

        mask = cv2.inRange(hsv, lower, upper)

        frame = cv2.blur(frame, (5,5))
        result = cv2.bitwise_and(frame, frame, mask = mask)

        cv2.imshow('result',result)

        k = cv2.waitKey(5) & 0xFF
        # escape or 'q' to exit
        if k == 27 or k == 113:
            break

    cap.release()

    cv2.destroyAllWindows()


if __name__ == "__main__":
    main(sys.argv[1:])
    