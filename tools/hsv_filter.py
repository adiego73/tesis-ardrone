#!/usr/bin/python

import cv2
import numpy as np
import sys

def nothing(x):
    pass

def main(argv):
    
    if len(argv) <= 0:
        sys.exit( "El primer argumento debe ser el numero de la camara. Ex. test@pc:~$ python hsv_filter.py 1" )
    
    cap = cv2.VideoCapture(int(argv[0]))

    # Creo la ventana
    name = 'Herramienta de configuracion de colores'
    cv2.namedWindow(name)

    # iniciliazo con 100 para evitar error
    h_min,s_min,v_min = 100,100,100
    h_max,s_max,v_max = 180,255,255

    # Creo el trackbar
    cv2.createTrackbar('H max', name,0,180,nothing)
    cv2.createTrackbar('H min', name,0,180,nothing)
    cv2.createTrackbar('S max', name,0,255,nothing)
    cv2.createTrackbar('S min', name,0,255,nothing)
    cv2.createTrackbar('V max', name,0,255,nothing)
    cv2.createTrackbar('V min', name,0,255,nothing)

    while(1):
        _, frame = cap.read()

        #convierto a HSV
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        # pido info de la trackbar
        h_min = cv2.getTrackbarPos('H min',name)
        s_min = cv2.getTrackbarPos('S min',name)
        v_min = cv2.getTrackbarPos('V min',name)
        
        h_max = cv2.getTrackbarPos('H max',name)
        s_max = cv2.getTrackbarPos('S max',name)
        v_max = cv2.getTrackbarPos('V max',name)

        # algoritmo de enmascaracion
        lower = np.array([h_min,s_min,v_min])
        upper = np.array([h_max,s_max,v_max])

        mask = cv2.inRange(hsv, lower, upper)

        frame = cv2.blur(frame, (5,5))
        result = cv2.bitwise_and(frame, frame, mask = mask)

        cv2.imshow(name,result)

        k = cv2.waitKey(5) & 0xFF
        # escape o 'q' para salir
        if k == 27 or k == 113:
            break

    cap.release()

    cv2.destroyAllWindows()


if __name__ == "__main__":
    main(sys.argv[1:])
    