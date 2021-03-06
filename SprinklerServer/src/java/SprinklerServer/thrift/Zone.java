/**
 * Autogenerated by Thrift Compiler (0.9.3)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
package SprinklerServer.thrift;

import org.apache.thrift.scheme.IScheme;
import org.apache.thrift.scheme.SchemeFactory;
import org.apache.thrift.scheme.StandardScheme;

import org.apache.thrift.scheme.TupleScheme;
import org.apache.thrift.protocol.TTupleProtocol;
import org.apache.thrift.protocol.TProtocolException;
import org.apache.thrift.EncodingUtils;
import org.apache.thrift.TException;
import org.apache.thrift.async.AsyncMethodCallback;
import org.apache.thrift.server.AbstractNonblockingServer.*;
import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import java.util.EnumMap;
import java.util.Set;
import java.util.HashSet;
import java.util.EnumSet;
import java.util.Collections;
import java.util.BitSet;
import java.nio.ByteBuffer;
import java.util.Arrays;
import javax.annotation.Generated;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

@SuppressWarnings({"cast", "rawtypes", "serial", "unchecked"})
@Generated(value = "Autogenerated by Thrift Compiler (0.9.3)", date = "2016-08-02")
public class Zone implements org.apache.thrift.TBase<Zone, Zone._Fields>, java.io.Serializable, Cloneable, Comparable<Zone> {
  private static final org.apache.thrift.protocol.TStruct STRUCT_DESC = new org.apache.thrift.protocol.TStruct("Zone");

  private static final org.apache.thrift.protocol.TField NUMBER_FIELD_DESC = new org.apache.thrift.protocol.TField("number", org.apache.thrift.protocol.TType.I32, (short)1);
  private static final org.apache.thrift.protocol.TField DURATION_FIELD_DESC = new org.apache.thrift.protocol.TField("duration", org.apache.thrift.protocol.TType.I32, (short)2);
  private static final org.apache.thrift.protocol.TField NAME_FIELD_DESC = new org.apache.thrift.protocol.TField("name", org.apache.thrift.protocol.TType.STRING, (short)3);
  private static final org.apache.thrift.protocol.TField ENABLED_FOR_CURRENT_PROGRAM_FIELD_DESC = new org.apache.thrift.protocol.TField("enabledForCurrentProgram", org.apache.thrift.protocol.TType.BOOL, (short)4);

  private static final Map<Class<? extends IScheme>, SchemeFactory> schemes = new HashMap<Class<? extends IScheme>, SchemeFactory>();
  static {
    schemes.put(StandardScheme.class, new ZoneStandardSchemeFactory());
    schemes.put(TupleScheme.class, new ZoneTupleSchemeFactory());
  }

  public int number; // required
  public int duration; // required
  public String name; // required
  public boolean enabledForCurrentProgram; // required

  /** The set of fields this struct contains, along with convenience methods for finding and manipulating them. */
  public enum _Fields implements org.apache.thrift.TFieldIdEnum {
    NUMBER((short)1, "number"),
    DURATION((short)2, "duration"),
    NAME((short)3, "name"),
    ENABLED_FOR_CURRENT_PROGRAM((short)4, "enabledForCurrentProgram");

    private static final Map<String, _Fields> byName = new HashMap<String, _Fields>();

    static {
      for (_Fields field : EnumSet.allOf(_Fields.class)) {
        byName.put(field.getFieldName(), field);
      }
    }

    /**
     * Find the _Fields constant that matches fieldId, or null if its not found.
     */
    public static _Fields findByThriftId(int fieldId) {
      switch(fieldId) {
        case 1: // NUMBER
          return NUMBER;
        case 2: // DURATION
          return DURATION;
        case 3: // NAME
          return NAME;
        case 4: // ENABLED_FOR_CURRENT_PROGRAM
          return ENABLED_FOR_CURRENT_PROGRAM;
        default:
          return null;
      }
    }

    /**
     * Find the _Fields constant that matches fieldId, throwing an exception
     * if it is not found.
     */
    public static _Fields findByThriftIdOrThrow(int fieldId) {
      _Fields fields = findByThriftId(fieldId);
      if (fields == null) throw new IllegalArgumentException("Field " + fieldId + " doesn't exist!");
      return fields;
    }

    /**
     * Find the _Fields constant that matches name, or null if its not found.
     */
    public static _Fields findByName(String name) {
      return byName.get(name);
    }

    private final short _thriftId;
    private final String _fieldName;

    _Fields(short thriftId, String fieldName) {
      _thriftId = thriftId;
      _fieldName = fieldName;
    }

    public short getThriftFieldId() {
      return _thriftId;
    }

    public String getFieldName() {
      return _fieldName;
    }
  }

  // isset id assignments
  private static final int __NUMBER_ISSET_ID = 0;
  private static final int __DURATION_ISSET_ID = 1;
  private static final int __ENABLEDFORCURRENTPROGRAM_ISSET_ID = 2;
  private byte __isset_bitfield = 0;
  public static final Map<_Fields, org.apache.thrift.meta_data.FieldMetaData> metaDataMap;
  static {
    Map<_Fields, org.apache.thrift.meta_data.FieldMetaData> tmpMap = new EnumMap<_Fields, org.apache.thrift.meta_data.FieldMetaData>(_Fields.class);
    tmpMap.put(_Fields.NUMBER, new org.apache.thrift.meta_data.FieldMetaData("number", org.apache.thrift.TFieldRequirementType.DEFAULT, 
        new org.apache.thrift.meta_data.FieldValueMetaData(org.apache.thrift.protocol.TType.I32)));
    tmpMap.put(_Fields.DURATION, new org.apache.thrift.meta_data.FieldMetaData("duration", org.apache.thrift.TFieldRequirementType.DEFAULT, 
        new org.apache.thrift.meta_data.FieldValueMetaData(org.apache.thrift.protocol.TType.I32)));
    tmpMap.put(_Fields.NAME, new org.apache.thrift.meta_data.FieldMetaData("name", org.apache.thrift.TFieldRequirementType.DEFAULT, 
        new org.apache.thrift.meta_data.FieldValueMetaData(org.apache.thrift.protocol.TType.STRING)));
    tmpMap.put(_Fields.ENABLED_FOR_CURRENT_PROGRAM, new org.apache.thrift.meta_data.FieldMetaData("enabledForCurrentProgram", org.apache.thrift.TFieldRequirementType.DEFAULT, 
        new org.apache.thrift.meta_data.FieldValueMetaData(org.apache.thrift.protocol.TType.BOOL)));
    metaDataMap = Collections.unmodifiableMap(tmpMap);
    org.apache.thrift.meta_data.FieldMetaData.addStructMetaDataMap(Zone.class, metaDataMap);
  }

  public Zone() {
  }

  public Zone(
    int number,
    int duration,
    String name,
    boolean enabledForCurrentProgram)
  {
    this();
    this.number = number;
    setNumberIsSet(true);
    this.duration = duration;
    setDurationIsSet(true);
    this.name = name;
    this.enabledForCurrentProgram = enabledForCurrentProgram;
    setEnabledForCurrentProgramIsSet(true);
  }

  /**
   * Performs a deep copy on <i>other</i>.
   */
  public Zone(Zone other) {
    __isset_bitfield = other.__isset_bitfield;
    this.number = other.number;
    this.duration = other.duration;
    if (other.isSetName()) {
      this.name = other.name;
    }
    this.enabledForCurrentProgram = other.enabledForCurrentProgram;
  }

  public Zone deepCopy() {
    return new Zone(this);
  }

  @Override
  public void clear() {
    setNumberIsSet(false);
    this.number = 0;
    setDurationIsSet(false);
    this.duration = 0;
    this.name = null;
    setEnabledForCurrentProgramIsSet(false);
    this.enabledForCurrentProgram = false;
  }

  public int getNumber() {
    return this.number;
  }

  public Zone setNumber(int number) {
    this.number = number;
    setNumberIsSet(true);
    return this;
  }

  public void unsetNumber() {
    __isset_bitfield = EncodingUtils.clearBit(__isset_bitfield, __NUMBER_ISSET_ID);
  }

  /** Returns true if field number is set (has been assigned a value) and false otherwise */
  public boolean isSetNumber() {
    return EncodingUtils.testBit(__isset_bitfield, __NUMBER_ISSET_ID);
  }

  public void setNumberIsSet(boolean value) {
    __isset_bitfield = EncodingUtils.setBit(__isset_bitfield, __NUMBER_ISSET_ID, value);
  }

  public int getDuration() {
    return this.duration;
  }

  public Zone setDuration(int duration) {
    this.duration = duration;
    setDurationIsSet(true);
    return this;
  }

  public void unsetDuration() {
    __isset_bitfield = EncodingUtils.clearBit(__isset_bitfield, __DURATION_ISSET_ID);
  }

  /** Returns true if field duration is set (has been assigned a value) and false otherwise */
  public boolean isSetDuration() {
    return EncodingUtils.testBit(__isset_bitfield, __DURATION_ISSET_ID);
  }

  public void setDurationIsSet(boolean value) {
    __isset_bitfield = EncodingUtils.setBit(__isset_bitfield, __DURATION_ISSET_ID, value);
  }

  public String getName() {
    return this.name;
  }

  public Zone setName(String name) {
    this.name = name;
    return this;
  }

  public void unsetName() {
    this.name = null;
  }

  /** Returns true if field name is set (has been assigned a value) and false otherwise */
  public boolean isSetName() {
    return this.name != null;
  }

  public void setNameIsSet(boolean value) {
    if (!value) {
      this.name = null;
    }
  }

  public boolean isEnabledForCurrentProgram() {
    return this.enabledForCurrentProgram;
  }

  public Zone setEnabledForCurrentProgram(boolean enabledForCurrentProgram) {
    this.enabledForCurrentProgram = enabledForCurrentProgram;
    setEnabledForCurrentProgramIsSet(true);
    return this;
  }

  public void unsetEnabledForCurrentProgram() {
    __isset_bitfield = EncodingUtils.clearBit(__isset_bitfield, __ENABLEDFORCURRENTPROGRAM_ISSET_ID);
  }

  /** Returns true if field enabledForCurrentProgram is set (has been assigned a value) and false otherwise */
  public boolean isSetEnabledForCurrentProgram() {
    return EncodingUtils.testBit(__isset_bitfield, __ENABLEDFORCURRENTPROGRAM_ISSET_ID);
  }

  public void setEnabledForCurrentProgramIsSet(boolean value) {
    __isset_bitfield = EncodingUtils.setBit(__isset_bitfield, __ENABLEDFORCURRENTPROGRAM_ISSET_ID, value);
  }

  public void setFieldValue(_Fields field, Object value) {
    switch (field) {
    case NUMBER:
      if (value == null) {
        unsetNumber();
      } else {
        setNumber((Integer)value);
      }
      break;

    case DURATION:
      if (value == null) {
        unsetDuration();
      } else {
        setDuration((Integer)value);
      }
      break;

    case NAME:
      if (value == null) {
        unsetName();
      } else {
        setName((String)value);
      }
      break;

    case ENABLED_FOR_CURRENT_PROGRAM:
      if (value == null) {
        unsetEnabledForCurrentProgram();
      } else {
        setEnabledForCurrentProgram((Boolean)value);
      }
      break;

    }
  }

  public Object getFieldValue(_Fields field) {
    switch (field) {
    case NUMBER:
      return getNumber();

    case DURATION:
      return getDuration();

    case NAME:
      return getName();

    case ENABLED_FOR_CURRENT_PROGRAM:
      return isEnabledForCurrentProgram();

    }
    throw new IllegalStateException();
  }

  /** Returns true if field corresponding to fieldID is set (has been assigned a value) and false otherwise */
  public boolean isSet(_Fields field) {
    if (field == null) {
      throw new IllegalArgumentException();
    }

    switch (field) {
    case NUMBER:
      return isSetNumber();
    case DURATION:
      return isSetDuration();
    case NAME:
      return isSetName();
    case ENABLED_FOR_CURRENT_PROGRAM:
      return isSetEnabledForCurrentProgram();
    }
    throw new IllegalStateException();
  }

  @Override
  public boolean equals(Object that) {
    if (that == null)
      return false;
    if (that instanceof Zone)
      return this.equals((Zone)that);
    return false;
  }

  public boolean equals(Zone that) {
    if (that == null)
      return false;

    boolean this_present_number = true;
    boolean that_present_number = true;
    if (this_present_number || that_present_number) {
      if (!(this_present_number && that_present_number))
        return false;
      if (this.number != that.number)
        return false;
    }

    boolean this_present_duration = true;
    boolean that_present_duration = true;
    if (this_present_duration || that_present_duration) {
      if (!(this_present_duration && that_present_duration))
        return false;
      if (this.duration != that.duration)
        return false;
    }

    boolean this_present_name = true && this.isSetName();
    boolean that_present_name = true && that.isSetName();
    if (this_present_name || that_present_name) {
      if (!(this_present_name && that_present_name))
        return false;
      if (!this.name.equals(that.name))
        return false;
    }

    boolean this_present_enabledForCurrentProgram = true;
    boolean that_present_enabledForCurrentProgram = true;
    if (this_present_enabledForCurrentProgram || that_present_enabledForCurrentProgram) {
      if (!(this_present_enabledForCurrentProgram && that_present_enabledForCurrentProgram))
        return false;
      if (this.enabledForCurrentProgram != that.enabledForCurrentProgram)
        return false;
    }

    return true;
  }

  @Override
  public int hashCode() {
    List<Object> list = new ArrayList<Object>();

    boolean present_number = true;
    list.add(present_number);
    if (present_number)
      list.add(number);

    boolean present_duration = true;
    list.add(present_duration);
    if (present_duration)
      list.add(duration);

    boolean present_name = true && (isSetName());
    list.add(present_name);
    if (present_name)
      list.add(name);

    boolean present_enabledForCurrentProgram = true;
    list.add(present_enabledForCurrentProgram);
    if (present_enabledForCurrentProgram)
      list.add(enabledForCurrentProgram);

    return list.hashCode();
  }

  @Override
  public int compareTo(Zone other) {
    if (!getClass().equals(other.getClass())) {
      return getClass().getName().compareTo(other.getClass().getName());
    }

    int lastComparison = 0;

    lastComparison = Boolean.valueOf(isSetNumber()).compareTo(other.isSetNumber());
    if (lastComparison != 0) {
      return lastComparison;
    }
    if (isSetNumber()) {
      lastComparison = org.apache.thrift.TBaseHelper.compareTo(this.number, other.number);
      if (lastComparison != 0) {
        return lastComparison;
      }
    }
    lastComparison = Boolean.valueOf(isSetDuration()).compareTo(other.isSetDuration());
    if (lastComparison != 0) {
      return lastComparison;
    }
    if (isSetDuration()) {
      lastComparison = org.apache.thrift.TBaseHelper.compareTo(this.duration, other.duration);
      if (lastComparison != 0) {
        return lastComparison;
      }
    }
    lastComparison = Boolean.valueOf(isSetName()).compareTo(other.isSetName());
    if (lastComparison != 0) {
      return lastComparison;
    }
    if (isSetName()) {
      lastComparison = org.apache.thrift.TBaseHelper.compareTo(this.name, other.name);
      if (lastComparison != 0) {
        return lastComparison;
      }
    }
    lastComparison = Boolean.valueOf(isSetEnabledForCurrentProgram()).compareTo(other.isSetEnabledForCurrentProgram());
    if (lastComparison != 0) {
      return lastComparison;
    }
    if (isSetEnabledForCurrentProgram()) {
      lastComparison = org.apache.thrift.TBaseHelper.compareTo(this.enabledForCurrentProgram, other.enabledForCurrentProgram);
      if (lastComparison != 0) {
        return lastComparison;
      }
    }
    return 0;
  }

  public _Fields fieldForId(int fieldId) {
    return _Fields.findByThriftId(fieldId);
  }

  public void read(org.apache.thrift.protocol.TProtocol iprot) throws org.apache.thrift.TException {
    schemes.get(iprot.getScheme()).getScheme().read(iprot, this);
  }

  public void write(org.apache.thrift.protocol.TProtocol oprot) throws org.apache.thrift.TException {
    schemes.get(oprot.getScheme()).getScheme().write(oprot, this);
  }

  @Override
  public String toString() {
    StringBuilder sb = new StringBuilder("Zone(");
    boolean first = true;

    sb.append("number:");
    sb.append(this.number);
    first = false;
    if (!first) sb.append(", ");
    sb.append("duration:");
    sb.append(this.duration);
    first = false;
    if (!first) sb.append(", ");
    sb.append("name:");
    if (this.name == null) {
      sb.append("null");
    } else {
      sb.append(this.name);
    }
    first = false;
    if (!first) sb.append(", ");
    sb.append("enabledForCurrentProgram:");
    sb.append(this.enabledForCurrentProgram);
    first = false;
    sb.append(")");
    return sb.toString();
  }

  public void validate() throws org.apache.thrift.TException {
    // check for required fields
    // check for sub-struct validity
  }

  private void writeObject(java.io.ObjectOutputStream out) throws java.io.IOException {
    try {
      write(new org.apache.thrift.protocol.TCompactProtocol(new org.apache.thrift.transport.TIOStreamTransport(out)));
    } catch (org.apache.thrift.TException te) {
      throw new java.io.IOException(te);
    }
  }

  private void readObject(java.io.ObjectInputStream in) throws java.io.IOException, ClassNotFoundException {
    try {
      // it doesn't seem like you should have to do this, but java serialization is wacky, and doesn't call the default constructor.
      __isset_bitfield = 0;
      read(new org.apache.thrift.protocol.TCompactProtocol(new org.apache.thrift.transport.TIOStreamTransport(in)));
    } catch (org.apache.thrift.TException te) {
      throw new java.io.IOException(te);
    }
  }

  private static class ZoneStandardSchemeFactory implements SchemeFactory {
    public ZoneStandardScheme getScheme() {
      return new ZoneStandardScheme();
    }
  }

  private static class ZoneStandardScheme extends StandardScheme<Zone> {

    public void read(org.apache.thrift.protocol.TProtocol iprot, Zone struct) throws org.apache.thrift.TException {
      org.apache.thrift.protocol.TField schemeField;
      iprot.readStructBegin();
      while (true)
      {
        schemeField = iprot.readFieldBegin();
        if (schemeField.type == org.apache.thrift.protocol.TType.STOP) { 
          break;
        }
        switch (schemeField.id) {
          case 1: // NUMBER
            if (schemeField.type == org.apache.thrift.protocol.TType.I32) {
              struct.number = iprot.readI32();
              struct.setNumberIsSet(true);
            } else { 
              org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
            }
            break;
          case 2: // DURATION
            if (schemeField.type == org.apache.thrift.protocol.TType.I32) {
              struct.duration = iprot.readI32();
              struct.setDurationIsSet(true);
            } else { 
              org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
            }
            break;
          case 3: // NAME
            if (schemeField.type == org.apache.thrift.protocol.TType.STRING) {
              struct.name = iprot.readString();
              struct.setNameIsSet(true);
            } else { 
              org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
            }
            break;
          case 4: // ENABLED_FOR_CURRENT_PROGRAM
            if (schemeField.type == org.apache.thrift.protocol.TType.BOOL) {
              struct.enabledForCurrentProgram = iprot.readBool();
              struct.setEnabledForCurrentProgramIsSet(true);
            } else { 
              org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
            }
            break;
          default:
            org.apache.thrift.protocol.TProtocolUtil.skip(iprot, schemeField.type);
        }
        iprot.readFieldEnd();
      }
      iprot.readStructEnd();

      // check for required fields of primitive type, which can't be checked in the validate method
      struct.validate();
    }

    public void write(org.apache.thrift.protocol.TProtocol oprot, Zone struct) throws org.apache.thrift.TException {
      struct.validate();

      oprot.writeStructBegin(STRUCT_DESC);
      oprot.writeFieldBegin(NUMBER_FIELD_DESC);
      oprot.writeI32(struct.number);
      oprot.writeFieldEnd();
      oprot.writeFieldBegin(DURATION_FIELD_DESC);
      oprot.writeI32(struct.duration);
      oprot.writeFieldEnd();
      if (struct.name != null) {
        oprot.writeFieldBegin(NAME_FIELD_DESC);
        oprot.writeString(struct.name);
        oprot.writeFieldEnd();
      }
      oprot.writeFieldBegin(ENABLED_FOR_CURRENT_PROGRAM_FIELD_DESC);
      oprot.writeBool(struct.enabledForCurrentProgram);
      oprot.writeFieldEnd();
      oprot.writeFieldStop();
      oprot.writeStructEnd();
    }

  }

  private static class ZoneTupleSchemeFactory implements SchemeFactory {
    public ZoneTupleScheme getScheme() {
      return new ZoneTupleScheme();
    }
  }

  private static class ZoneTupleScheme extends TupleScheme<Zone> {

    @Override
    public void write(org.apache.thrift.protocol.TProtocol prot, Zone struct) throws org.apache.thrift.TException {
      TTupleProtocol oprot = (TTupleProtocol) prot;
      BitSet optionals = new BitSet();
      if (struct.isSetNumber()) {
        optionals.set(0);
      }
      if (struct.isSetDuration()) {
        optionals.set(1);
      }
      if (struct.isSetName()) {
        optionals.set(2);
      }
      if (struct.isSetEnabledForCurrentProgram()) {
        optionals.set(3);
      }
      oprot.writeBitSet(optionals, 4);
      if (struct.isSetNumber()) {
        oprot.writeI32(struct.number);
      }
      if (struct.isSetDuration()) {
        oprot.writeI32(struct.duration);
      }
      if (struct.isSetName()) {
        oprot.writeString(struct.name);
      }
      if (struct.isSetEnabledForCurrentProgram()) {
        oprot.writeBool(struct.enabledForCurrentProgram);
      }
    }

    @Override
    public void read(org.apache.thrift.protocol.TProtocol prot, Zone struct) throws org.apache.thrift.TException {
      TTupleProtocol iprot = (TTupleProtocol) prot;
      BitSet incoming = iprot.readBitSet(4);
      if (incoming.get(0)) {
        struct.number = iprot.readI32();
        struct.setNumberIsSet(true);
      }
      if (incoming.get(1)) {
        struct.duration = iprot.readI32();
        struct.setDurationIsSet(true);
      }
      if (incoming.get(2)) {
        struct.name = iprot.readString();
        struct.setNameIsSet(true);
      }
      if (incoming.get(3)) {
        struct.enabledForCurrentProgram = iprot.readBool();
        struct.setEnabledForCurrentProgramIsSet(true);
      }
    }
  }

}

