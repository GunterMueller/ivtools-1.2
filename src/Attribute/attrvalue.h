/*
 * Copyright (c) 1994-1999 Vectaport Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the names of the copyright holders not be used in
 * advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  The copyright holders make
 * no representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 *
 * THE COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY SPECIAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING
 * FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
 * WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 */

#if !defined(_attrvalue_h)
#define _attrvalue_h

#include <stdlib.h>
#include <OS/enter-scope.h>

extern "C" {
    int symbol_add(char*);
    int symbol_del(int);
    int symbol_find(char*);
    char* symbol_pntr(int);
}

class AttributeValueList;
class ostream;

//: void* pointer plus object classid (see macro in OverlayUnidraw/ovcomps.h)
// used in attr_value.
typedef struct {
       void *ptr;
       unsigned int type;
} objval_struct;

//: pointer to list of values, plus optional type id.
// used in attr_value.
typedef struct {
  AttributeValueList *ptr;
  unsigned int type;
} arrayval_struct;

//: keyword symbol id, plus number of arguments that follow.
// used in attr_value.
typedef struct {
  unsigned int keyid;
  unsigned int keynarg;
} keyval_struct;

//: union for AttributeValue typed data storage.
typedef union attr_value_union
{
      char              charval;
      unsigned char     ucharval;
      short             shortval;
      unsigned short    ushortval;
      int               dfintval;
      unsigned int      dfunsval;
      long              lnintval;
      unsigned long     lnunsval;
      float             floatval;
      double            doublval;
      unsigned int      symbolid;
      objval_struct     objval;
      arrayval_struct   arrayval;
      keyval_struct     keyval;
} attr_value;

typedef const char* const_char_ptr;

//: multi-type attribute value object.
class AttributeValue {
public:
    enum ValueType { UnknownType, CharType, UCharType, ShortType, UShortType, 
		     IntType, UIntType, LongType, ULongType, FloatType, DoubleType, 
                     StringType, SymbolType, ArrayType, StreamType, CommandType, KeywordType, 
                     ObjectType, EofType, BooleanType, OperatorType, BlankType };
    // enum for attribute value types.

    AttributeValue(ValueType type);
    // construct with specified type and unitialized value.
    AttributeValue(ValueType type, attr_value value);
    // construct with specified type and value struct.
    AttributeValue(AttributeValue&);
    // copy constructor.
    AttributeValue();
    // default constructor (UnknownType constructor).

    AttributeValue(char);
    // CharType constructor.
    AttributeValue(unsigned char);
    // UCharType constructor.
    AttributeValue(short);
    // ShortType constructor.
    AttributeValue(unsigned short);
    // UShortType constructor.
    AttributeValue(int, ValueType);
    // IntType constructor or any other int-like value.
    AttributeValue(unsigned int, ValueType);
    // UIntType constructor or any other unsigned-int-like value including SymbolType.
    AttributeValue(unsigned int, unsigned int, ValueType=KeywordType);
    // KeywordType constructor (or can be used for ObjectType).
    AttributeValue(long);
    // LongType constructor.
    AttributeValue(unsigned long);
    // ULongType constructor.
    AttributeValue(float);
    // FloatType constructor.
    AttributeValue(double);
    // DoubleType constructor.
    AttributeValue(int class_symid, void*);
    // ObjectType constructor.
    AttributeValue(AttributeValueList*);
    // ArrayType constructor.
    AttributeValue(const char*);
    // StringType constructor.

    virtual ~AttributeValue();
    // set to UnknownType and unref pointer if ArrayType.

    AttributeValue& operator= (const AttributeValue&);
    // copy assignment operator.

    ValueType type() const;
    // return type enum.
    void type(ValueType);
    // set type enum.
    ValueType aggregate_type() const;
    // set type used for aggregate values (ArrayType, StreamType).
    int type_size() { return type_size(type()); }
    // return sizeof of value of this type.
    static int type_size(ValueType);
    // return sizeof of value of given type.

    char& char_ref();                 // char by reference.
    unsigned char& uchar_ref();       // unsigned char by reference.
    short& short_ref();               // short by reference.
    unsigned short& ushort_ref();     // unsigned short by reference.
    int& int_ref();                   // int by reference.
    unsigned int& uint_ref();         // unsigned int by reference.
    boolean& boolean_ref();           // boolean by reference.
    long& long_ref();                 // long by reference.
    unsigned long& ulong_ref();       // unsigned long by reference.
    float& float_ref();               // float by reference.
    double& double_ref();             // double by reference.
    unsigned int& string_ref();       // string symbol id by reference.
    unsigned int& symbol_ref();       // symbol id by reference.
    void*& obj_ref();                 // void* pointer to object by reference.
    unsigned int& obj_type_ref();     // classid of object by reference.
    AttributeValueList*& array_ref(); // values in list by reference.
    unsigned int& array_type_ref();   // type of values in list by reference
    unsigned int& keyid_ref();        // symbol id of keyword by reference.
    unsigned int& keynarg_ref();      // number of arguments after keyword by reference.

    char char_val();                  // char by value.                             
    unsigned char uchar_val();	      // unsigned char by value.                    
    short short_val();		      // short by value.                            
    unsigned short ushort_val();      // unsigned short by value.                   
    int int_val();		      // int by value.                              
    unsigned int uint_val();	      // unsigned int by value.                     
    boolean boolean_val();	      // boolean by value.                          
    long long_val();		      // long by value.                             
    unsigned long ulong_val();	      // unsigned long by value.                    
    float float_val();		      // float by value.                            
    double double_val();	      // double by value.                           
    unsigned int string_val();	      // string symbol id by value.                 
    unsigned int symbol_val();	      // symbol id by value.                        
    void* obj_val();		      // void* pointer to object by value.          
    unsigned int obj_type_val();      // classid of object by value.                
    AttributeValueList* array_val();  // values in list by value.                   
    unsigned int array_type_val();    // type of values in list by value            
    unsigned int keyid_val();	      // symbol id of keyword by value.             
    unsigned int keynarg_val();	      // number of arguments after keyword by value.

    const char* string_ptr();
    // lookup and return pointer to string associated with string.
    const char* symbol_ptr();
    // lookup and return pointer to string associated with symbol.
    int array_len();
    // length of list of values when ArrayType.

    unsigned int command_symid();
    // symbol id of associated command name, for use with ComTerp.
    void command_symid(unsigned int);
    // set symbol id of associated command name, for use with ComTerp.

    void negate();
    // negate numeric values.

    boolean is_true() { return type() != UnknownType && boolean_val(); }
    // returns true if !UnknownType && boolean_val()
    boolean is_false() { return !is_true(); }
    // returns true if !is_true()
    boolean is_type(ValueType t) { return type() == t; }
    // returns true if type() == 't'.

    boolean is_char() { return is_char(type()); }
    // returns true if CharType || UCharType.
    boolean is_short() { return is_short(type()); }
    // returns true if ShortType || UShortType.
    boolean is_int() { return is_int(type()); }
    // returns true if IntType || UIntType.
    boolean is_long() { return is_long(type()); }
    // returns true if LongType || ULongType.
    boolean is_float() { return is_float(type()); }
    // returns true if FloatType.
    boolean is_double() { return is_double(type()); }
    // returns true if DoubleType.

    boolean is_integer() { return is_integer(type()); }
    // returns true if is_char() || is_short() || is_int() || is_long().
    boolean is_floatingpoint() { return is_floatingpoint(type()); }
    // returns true if is_float() || is_double().
    boolean is_num() { return is_integer(type()) || is_floatingpoint(type()); }
    // returns true if is_integer() || is_floatingpoint().
    boolean is_numeric() { return is_num(); }
    // same as AttributeValue::is_num().

    boolean is_array() { return is_type(ArrayType); }
    // returns true if ArrayType.
    boolean is_unknown() { return is_type(UnknownType); }
    // returns true if UnknownType.
    boolean is_null() { return is_unknown(); }
    // returns true if UnknownType.
    boolean is_nil() { return is_unknown(); }
    // returns true if UnknownType.
    boolean is_known() { return !is_type(UnknownType); }
    // returns true if !UnknownType.
    boolean is_string() { return is_type(StringType) || is_type(SymbolType); }
    // returns true if StringType || SymbolType.
    boolean is_command() { return is_type(CommandType); }
    // returns true if CommandType (for use of ComTerp).
    boolean is_object() { return is_type(ObjectType); }
    // returns true if ObjectType.

    static boolean is_char(ValueType t) 
      { return t==CharType || t==UCharType; }
    static boolean is_short(ValueType t)
      { return t==ShortType || t==UShortType; }
    static boolean is_int(ValueType t)
      { return t==IntType || t==UIntType; }
    static boolean is_long(ValueType t) 
      { return t==LongType || t==ULongType; }
    static boolean is_float(ValueType t)
      { return t==FloatType; }
    static boolean is_double(ValueType t)
      { return t==DoubleType; }

    static boolean is_integer(ValueType t)
      { return is_char(t) || is_short(t) || is_int(t) | is_long(t); }
    static boolean is_floatingpoint(ValueType t)
      { return is_float(t) || is_double(t); }
    static boolean is_num(ValueType t)
      { return is_integer(t) || is_floatingpoint(t); }


    boolean is_blank() { return is_type(BlankType); }
    // returns true if BlankType.
    static boolean is_blank(ValueType t)
      { return t==BlankType; };

    friend ostream& operator << (ostream& s, const AttributeValue&);
    // output AttributeValue to ostream.

    void* value_ptr() { return &_v; }
    // returns void* pointer to value struct.

protected:
    ValueType _type;
    attr_value _v;
    union { 
      ValueType _aggregate_type; // used for ArrayType.
      unsigned int _command_symid; // used for CommandType.
    };

};

//: for quick casting in debugger
typedef class AttributeValue _AV; 

#endif /* !defined(_attrvalue_h) */
