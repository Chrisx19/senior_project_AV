// Auto-generated. Do not edit!

// (in-package av_senior.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class Arduino_telemetry {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.Right_Distance = null;
      this.Mid_Distance = null;
      this.Left_Distance = null;
    }
    else {
      if (initObj.hasOwnProperty('Right_Distance')) {
        this.Right_Distance = initObj.Right_Distance
      }
      else {
        this.Right_Distance = 0;
      }
      if (initObj.hasOwnProperty('Mid_Distance')) {
        this.Mid_Distance = initObj.Mid_Distance
      }
      else {
        this.Mid_Distance = 0;
      }
      if (initObj.hasOwnProperty('Left_Distance')) {
        this.Left_Distance = initObj.Left_Distance
      }
      else {
        this.Left_Distance = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type Arduino_telemetry
    // Serialize message field [Right_Distance]
    bufferOffset = _serializer.int16(obj.Right_Distance, buffer, bufferOffset);
    // Serialize message field [Mid_Distance]
    bufferOffset = _serializer.int16(obj.Mid_Distance, buffer, bufferOffset);
    // Serialize message field [Left_Distance]
    bufferOffset = _serializer.int16(obj.Left_Distance, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type Arduino_telemetry
    let len;
    let data = new Arduino_telemetry(null);
    // Deserialize message field [Right_Distance]
    data.Right_Distance = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [Mid_Distance]
    data.Mid_Distance = _deserializer.int16(buffer, bufferOffset);
    // Deserialize message field [Left_Distance]
    data.Left_Distance = _deserializer.int16(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 6;
  }

  static datatype() {
    // Returns string type for a message object
    return 'av_senior/Arduino_telemetry';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'e6abe48c358f1e845717a29449a49bcb';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    int16 Right_Distance
    int16 Mid_Distance
    int16 Left_Distance
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new Arduino_telemetry(null);
    if (msg.Right_Distance !== undefined) {
      resolved.Right_Distance = msg.Right_Distance;
    }
    else {
      resolved.Right_Distance = 0
    }

    if (msg.Mid_Distance !== undefined) {
      resolved.Mid_Distance = msg.Mid_Distance;
    }
    else {
      resolved.Mid_Distance = 0
    }

    if (msg.Left_Distance !== undefined) {
      resolved.Left_Distance = msg.Left_Distance;
    }
    else {
      resolved.Left_Distance = 0
    }

    return resolved;
    }
};

module.exports = Arduino_telemetry;
