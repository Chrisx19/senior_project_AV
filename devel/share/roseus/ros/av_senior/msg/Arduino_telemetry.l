;; Auto-generated. Do not edit!


(when (boundp 'av_senior::Arduino_telemetry)
  (if (not (find-package "AV_SENIOR"))
    (make-package "AV_SENIOR"))
  (shadow 'Arduino_telemetry (find-package "AV_SENIOR")))
(unless (find-package "AV_SENIOR::ARDUINO_TELEMETRY")
  (make-package "AV_SENIOR::ARDUINO_TELEMETRY"))

(in-package "ROS")
;;//! \htmlinclude Arduino_telemetry.msg.html


(defclass av_senior::Arduino_telemetry
  :super ros::object
  :slots (_Right_Distance _Mid_Distance _Left_Distance ))

(defmethod av_senior::Arduino_telemetry
  (:init
   (&key
    ((:Right_Distance __Right_Distance) 0)
    ((:Mid_Distance __Mid_Distance) 0)
    ((:Left_Distance __Left_Distance) 0)
    )
   (send-super :init)
   (setq _Right_Distance (round __Right_Distance))
   (setq _Mid_Distance (round __Mid_Distance))
   (setq _Left_Distance (round __Left_Distance))
   self)
  (:Right_Distance
   (&optional __Right_Distance)
   (if __Right_Distance (setq _Right_Distance __Right_Distance)) _Right_Distance)
  (:Mid_Distance
   (&optional __Mid_Distance)
   (if __Mid_Distance (setq _Mid_Distance __Mid_Distance)) _Mid_Distance)
  (:Left_Distance
   (&optional __Left_Distance)
   (if __Left_Distance (setq _Left_Distance __Left_Distance)) _Left_Distance)
  (:serialization-length
   ()
   (+
    ;; int16 _Right_Distance
    2
    ;; int16 _Mid_Distance
    2
    ;; int16 _Left_Distance
    2
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int16 _Right_Distance
       (write-word _Right_Distance s)
     ;; int16 _Mid_Distance
       (write-word _Mid_Distance s)
     ;; int16 _Left_Distance
       (write-word _Left_Distance s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int16 _Right_Distance
     (setq _Right_Distance (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _Mid_Distance
     (setq _Mid_Distance (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _Left_Distance
     (setq _Left_Distance (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;;
   self)
  )

(setf (get av_senior::Arduino_telemetry :md5sum-) "e6abe48c358f1e845717a29449a49bcb")
(setf (get av_senior::Arduino_telemetry :datatype-) "av_senior/Arduino_telemetry")
(setf (get av_senior::Arduino_telemetry :definition-)
      "int16 Right_Distance
int16 Mid_Distance
int16 Left_Distance

")



(provide :av_senior/Arduino_telemetry "e6abe48c358f1e845717a29449a49bcb")


