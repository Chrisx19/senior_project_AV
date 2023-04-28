#!/usr/bin/env python3
import rospy
import cv2
import time
from std_msgs.msg import Float32

#thres = 0.45 # Threshold to detect object

classNames = []
classFile = "/home/jetson/Desktop/Object_Detection_Files/coco.names"
with open(classFile,"rt") as f:
    classNames = f.read().rstrip("\n").split("\n")

configPath = "/home/jetson/Desktop/Object_Detection_Files/ssd_mobilenet_v3_large_coco_2020_01_14.pbtxt"
weightsPath = "/home/jetson/Desktop/Object_Detection_Files/frozen_inference_graph.pb"

net = cv2.dnn_DetectionModel(weightsPath,configPath)
net.setInputSize(320,320)
net.setInputScale(1.0/ 127.5)
net.setInputMean((127.5, 127.5, 127.5))
net.setInputSwapRB(True)

prev_frame_time = 0
new_frame_time = 0

rospy.init_node("object_detection", anonymous=False)
percent_person_pub = rospy.Publisher("/person", Float32, queue_size=10)

def getObjects(img, thres, nms, draw=True, objects=[]):
    classIds, confs, bbox = net.detect(img,confThreshold=thres,nmsThreshold=nms)
    #print(classIds,bbox)

    if len(objects) == 0: objects = classNames
    objectInfo =[]
    if len(classIds) != 0:
        for classId, confidence,box in zip(classIds.flatten(),confs.flatten(),bbox):
            className = classNames[classId - 1]
            if className in objects:
                objectInfo.append([box,className])
                if (draw):
                    cv2.rectangle(img,box,color=(0,255,0),thickness=2)
                    cv2.putText(img,classNames[classId-1].upper(),(box[0]+10,box[1]+30),
                    cv2.FONT_HERSHEY_COMPLEX,1,(0,255,0),2)
                    cv2.putText(img,str(round(confidence* 100,2)),(box[0]+200,box[1]+30),
                    cv2.FONT_HERSHEY_COMPLEX,1,(0,255,0),2)

                    percent_person_pub.publish(confidence * 100)
                    

    return img,objectInfo


if __name__ == "__main__":
    cap = cv2.VideoCapture(0)
    cap.set(3,640)
    cap.set(4,480)
    #cap.set(10,70)

    while not rospy.is_shutdown():
        success, img = cap.read()

        font = cv2.FONT_HERSHEY_COMPLEX
        new_frame_time = time.time()
        fps = 1 /(new_frame_time - prev_frame_time)
        prev_frame_time = new_frame_time
        fps = int(fps)
        fps = str(fps)

        cv2.putText(img, fps, (19, 25), font, 0.8, (0, 255, 255), 1, cv2.LINE_AA)

        result, objectInfo = getObjects(img,0.50,0.2, objects=['person'])

        if (not objectInfo):
            percent_person_pub.publish(0)
        # print(objectInfo)
        
        cv2.imshow("Output",img)
        cv2.waitKey(1)
        rospy.sleep(0.1)