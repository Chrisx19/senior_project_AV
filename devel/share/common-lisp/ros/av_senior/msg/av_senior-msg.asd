
(cl:in-package :asdf)

(defsystem "av_senior-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "Arduino_telemetry" :depends-on ("_package_Arduino_telemetry"))
    (:file "_package_Arduino_telemetry" :depends-on ("_package"))
  ))