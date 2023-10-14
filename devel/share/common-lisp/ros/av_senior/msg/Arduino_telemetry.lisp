; Auto-generated. Do not edit!


(cl:in-package av_senior-msg)


;//! \htmlinclude Arduino_telemetry.msg.html

(cl:defclass <Arduino_telemetry> (roslisp-msg-protocol:ros-message)
  ((Right_Distance
    :reader Right_Distance
    :initarg :Right_Distance
    :type cl:fixnum
    :initform 0)
   (Mid_Distance
    :reader Mid_Distance
    :initarg :Mid_Distance
    :type cl:fixnum
    :initform 0)
   (Left_Distance
    :reader Left_Distance
    :initarg :Left_Distance
    :type cl:fixnum
    :initform 0))
)

(cl:defclass Arduino_telemetry (<Arduino_telemetry>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Arduino_telemetry>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Arduino_telemetry)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name av_senior-msg:<Arduino_telemetry> is deprecated: use av_senior-msg:Arduino_telemetry instead.")))

(cl:ensure-generic-function 'Right_Distance-val :lambda-list '(m))
(cl:defmethod Right_Distance-val ((m <Arduino_telemetry>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader av_senior-msg:Right_Distance-val is deprecated.  Use av_senior-msg:Right_Distance instead.")
  (Right_Distance m))

(cl:ensure-generic-function 'Mid_Distance-val :lambda-list '(m))
(cl:defmethod Mid_Distance-val ((m <Arduino_telemetry>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader av_senior-msg:Mid_Distance-val is deprecated.  Use av_senior-msg:Mid_Distance instead.")
  (Mid_Distance m))

(cl:ensure-generic-function 'Left_Distance-val :lambda-list '(m))
(cl:defmethod Left_Distance-val ((m <Arduino_telemetry>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader av_senior-msg:Left_Distance-val is deprecated.  Use av_senior-msg:Left_Distance instead.")
  (Left_Distance m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Arduino_telemetry>) ostream)
  "Serializes a message object of type '<Arduino_telemetry>"
  (cl:let* ((signed (cl:slot-value msg 'Right_Distance)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'Mid_Distance)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'Left_Distance)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Arduino_telemetry>) istream)
  "Deserializes a message object of type '<Arduino_telemetry>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'Right_Distance) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'Mid_Distance) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'Left_Distance) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Arduino_telemetry>)))
  "Returns string type for a message object of type '<Arduino_telemetry>"
  "av_senior/Arduino_telemetry")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Arduino_telemetry)))
  "Returns string type for a message object of type 'Arduino_telemetry"
  "av_senior/Arduino_telemetry")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Arduino_telemetry>)))
  "Returns md5sum for a message object of type '<Arduino_telemetry>"
  "e6abe48c358f1e845717a29449a49bcb")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Arduino_telemetry)))
  "Returns md5sum for a message object of type 'Arduino_telemetry"
  "e6abe48c358f1e845717a29449a49bcb")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Arduino_telemetry>)))
  "Returns full string definition for message of type '<Arduino_telemetry>"
  (cl:format cl:nil "int16 Right_Distance~%int16 Mid_Distance~%int16 Left_Distance~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Arduino_telemetry)))
  "Returns full string definition for message of type 'Arduino_telemetry"
  (cl:format cl:nil "int16 Right_Distance~%int16 Mid_Distance~%int16 Left_Distance~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Arduino_telemetry>))
  (cl:+ 0
     2
     2
     2
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Arduino_telemetry>))
  "Converts a ROS message object to a list"
  (cl:list 'Arduino_telemetry
    (cl:cons ':Right_Distance (Right_Distance msg))
    (cl:cons ':Mid_Distance (Mid_Distance msg))
    (cl:cons ':Left_Distance (Left_Distance msg))
))
