/**
 * Autogenerated by Thrift Compiler (0.9.3)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
package SprinklerServer.thrift;


import java.util.Map;
import java.util.HashMap;
import org.apache.thrift.TEnum;

public enum Day implements org.apache.thrift.TEnum {
  SUNDAY(1),
  MONDAY(2),
  TUESDAY(3),
  WEDNESDAY(4),
  THURSDAY(5),
  FRIDAY(6),
  SATURDAY(7);

  private final int value;

  private Day(int value) {
    this.value = value;
  }

  /**
   * Get the integer value of this enum value, as defined in the Thrift IDL.
   */
  public int getValue() {
    return value;
  }

  /**
   * Find a the enum type by its integer value, as defined in the Thrift IDL.
   * @return null if the value is not found.
   */
  public static Day findByValue(int value) { 
    switch (value) {
      case 1:
        return SUNDAY;
      case 2:
        return MONDAY;
      case 3:
        return TUESDAY;
      case 4:
        return WEDNESDAY;
      case 5:
        return THURSDAY;
      case 6:
        return FRIDAY;
      case 7:
        return SATURDAY;
      default:
        return null;
    }
  }
}
