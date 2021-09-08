# Untitled - By: matth - Fri Dec 25 2020

import sensor, image, time, pyb, math
from pyb import Servo, UART

uart = UART(3, 115200, timeout_char=1000)
pan = Servo(1) # P7 Tilt
tilt = Servo(2) # P8 Pan
pan.calibration(540, 2550, 570, 1550, 2200)
tilt.calibration(540, 2550, 570, 1380, 2200)

threshold_index = 0 # 0 for red, 1 for green, 2 for blue

# Color Tracking Thresholds (L Min, L Max, A Min, A Max, B Min, B Max)
# The below thresholds track in general red/green/blue things. You may wish to tune them...
thresholds = [(30, 100, 15, 127, 15, 127), # generic_red_thresholds
              (30, 100, -64, -8, -32, 32), # generic_green_thresholds
              (0, 30, 0, 64, -128, 0)] # generic_blue_thresholds
imagecentre = (160, 120)

# set PID values for panning
panP = 0.065
panI = 0.06
panD = 0.02
# set PID values for tilting
tiltP = 0.09
tiltI = 0.09
tiltD = 0.03

panAngle = 0
tiltAngle = 0

motorP = 0.01
area_list = [0,0,0]
k = 0
cmd = 'd'
sensor.reset()

sensor.set_hmirror(True)
sensor.set_vflip(True)
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_auto_gain(False, set_db = 50) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
sensor.skip_frames(time = 2000)

clock = time.clock()
start_time = pyb.millis()

class PID:
    """ Simple PID control.

        This class implements a simplistic PID control algorithm. When first
        instantiated all the gain variables are set to zero, so calling
        the method GenOut will just return zero.
    """
    def __init__(self, Kp = 1, Kd = 0, Ki = 0):
        # initialze gains
        self.Kp = Kp
        self.Kd = Kd
        self.Ki = Ki

        self.Initialize()

    def SetKp(self, invar):
        """ Set proportional gain. """
        self.Kp = invar

    def SetKi(self, invar):
        """ Set integral gain. """
        self.Ki = invar

    def SetKd(self, invar):
        """ Set derivative gain. """
        self.Kd = invar

    def SetPrevErr(self, preverr):
        """ Set previous error value. """
        self.prev_err = preverr

    def Initialize(self):
        # initialize delta t variables
        self.currtm = time.time()
        self.prevtm = self.currtm

        self.prev_err = 0

        # term result variables
        self.Cp = 0
        self.Ci = 0
        self.Cd = 0


    def update(self, error, debug = 0):
        """ Performs a PID computation and returns a control value based on
            the elapsed time (dt) and the error signal from a summing junction
            (the error parameter).
        """


        self.currtm = time.time()               # get t
        dt = self.currtm - self.prevtm          # get delta t
        de = error - self.prev_err

        self.Cp = error               # proportional term

        self.Ci += error * dt                   # integral term

        self.Cd = 0
        if dt > 0:                              # no div by zero
            self.Cd = de/dt                     # derivative term


        self.prevtm = self.currtm               # save t for next pass
        self.prev_err = error                   # save t-1 error

        # sum the terms and return the result
        # terms_sum = self.Cp * self.Kp  + (self.Ki * self.Ci) + (self.Kd * self.Cd)
        terms_sum = self.Cp * self.Kp  + (self.Kd * self.Cd)
        return terms_sum

def updateXY(img):
    max_area = 0
    blob_cx = img.width()/2
    blob_cy = img.height()/2
    for blob in img.find_blobs([thresholds[threshold_index]], pixels_threshold=200, area_threshold=200, merge=True):
        img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(), blob.cy())
        # Note - the blob rotation is unique to 0-180 only.
        if blob.area() > max_area:
            max_area = blob.area()
            blob_cx = blob.cx()
            blob_cy = blob.cy()

    if max_area > 0:
        object_found = True
    else:
        object_found = False

    return ((blob_cx, blob_cy), object_found, max_area)

def pid_process(output, p, i, d, objCoord, centerCoord):
    # create a PID and initialize it
    p = PID(p, i, d)
    p.Initialize()

    # calculate the error
    error1 = centerCoord - objCoord
    # update the value
    output = p.update(error1)
    return output

def follow_object(panAngle, p, area_avg):
    followError = 90 - panAngle

    forwardSpeed = 300 - area_avg/120
    if forwardSpeed > 300:
        forwardSpeed = 300
    elif forwardSpeed < -100:
        forwardSpeed = -100
    if area == 0:
        forwardSpeed = 0

    steer_diff = (followError + followError*forwardSpeed*p)

    leftSpeed = forwardSpeed + steer_diff
    if leftSpeed >= 300:
        leftSpeed = 300
    elif leftSpeed <= -300:
        leftSpeed = -300



    rightSpeed = forwardSpeed - steer_diff
    if rightSpeed >= 300:
        rightSpeed = 300
    elif rightSpeed <= -300:
        rightSpeed = -300

    return (leftSpeed, rightSpeed)


while(True):
    clock.tick()
    frame = sensor.snapshot()
    # Get object Location
    objLoc = updateXY(frame)
    ((objx,objy), obj_found, area) = objLoc
    # Calculate Pan Angle with PID
    panAngle = pid_process(panAngle, panP, panI, panD, objx, 160)
    tiltAngle = pid_process(tiltAngle, tiltP, tiltI, tiltD, objy,120)
    # panAngle = -1 * panAngle
    # Calculate Tilt Angle with PID
    # Actuate Servos
    # if the pan angle is within the range, pan
    if pyb.millis() - start_time > 0:
        start_time = pyb.millis()
        if obj_found == True:
            if pan.angle() + panAngle > 0 and pan.angle() + panAngle < 180:
                pan.angle(pan.angle() + panAngle)
            if (tilt.angle() + tiltAngle) > 40 and (tilt.angle() + tiltAngle) < 160:
                tilt.angle(tilt.angle() + tiltAngle)
        else:
           pan.angle(90)
           tilt.angle(90)

    if len(area_list) > 7:
       area_list.pop(0)
    else:
       area_list.append(area)
    area_avg = sum(area_list)/len(area_list)


    (leftSpeed, rightSpeed) = follow_object(pan.angle() + panAngle, motorP, area_avg)
    s = '<'
    s += cmd
    s += 'e'
    s += 'p'
    s += ','
    s += str(leftSpeed)
    s += ','
    s += str(rightSpeed)
    s += '>'
    uart.write(s.encode());

    print("L: ", leftSpeed,"R: ", rightSpeed, " ", cmd)
    print(clock.fps(), sensor.get_gain_db())
