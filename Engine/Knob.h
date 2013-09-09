//  Powiter
//
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
/*
 *Created by Alexandre GAUTHIER-FOICHAT on 6/1/2012.
 *contact: immarespond at gmail dot com
 *
 */

#ifndef KNOB_H
#define KNOB_H

#include <vector>
#include <string>
#include <map>

#include <QtCore/QVariant>
#include <QtCore/QStringList>

#include "Global/GlobalDefines.h"

#include "Engine/Singleton.h"

class Knob;
class Node;

/******************************VARIANT**************************************/

/*Variant type used by knob storage type.
 Do not use explicit in front of the constructors to
 allow type conversion!. This API should be
 transparant for the plug-in programmer.*/
class Variant{
    
    
    QVariant* _variant;
    
public:
    
    Variant():
    _variant(new QVariant)
    {
        
    }
    
    Variant(const Variant& other)
    :_variant(other._variant)
    {
        
    }
    
    Variant(const QVariant& qtVariant):
    _variant(new QVariant(qtVariant))
    {
        
    }
    
    Variant(const std::string& str):
    _variant(new QVariant(str.c_str()))
    {
        
    }
    
    template<typename T>
    Variant(T variant[] ,int count)
    {
        QList<QVariant> list;
        for (int i = 0; i < count; ++i) {
            list << QVariant(variant[i]);
        }
        _variant = new QVariant(list);
    }
    
    virtual ~Variant(){ /*delete _variant;*/ }
    
    template<typename T>
    T value(){
        return _variant->value<T>();
    }
    
    const QVariant& getQVariant() const {
        return *_variant;
    }
    
    void setValue(const Variant& variant){
        _variant->setValue(variant.getQVariant());
    }
    
    void setValue(QVariant qtVariant){
        _variant->setValue(qtVariant);
    }
    
    void setValue(const QVariant& qtVariant){
        _variant->setValue(qtVariant);
    }
    
    void setValue(const std::string& str){
        _variant->setValue(QString(str.c_str()));
    }
    template<typename T>
    void setValue(T variant[],int count){
        QList<QVariant> list;
        for (int i = 0; i < count; ++i) {
            list << QVariant(variant[i]);
        }
        _variant->setValue(list);
    }
    
    bool isNull() const{return _variant->isNull();}
};
Q_DECLARE_METATYPE(Variant);


namespace Powiter {
    class LibraryBinary;
}

typedef unsigned int Knob_Mask;
/******************************KNOB_FACTORY**************************************/


class KnobFactory : public Singleton<KnobFactory>{
    
    std::map<std::string,Powiter::LibraryBinary*> _loadedKnobs;
    
    void loadKnobPlugins();
    
    void loadBultinKnobs();
    
public:
    
    
    KnobFactory();
    
    virtual ~KnobFactory();
    
    const std::map<std::string,Powiter::LibraryBinary*>& getLoadedKnobs(){return _loadedKnobs;}
    
    /*Calls the unique instance of the KnobFactory and
     calls the appropriate pointer to function to create a knob.*/
    static Knob* createKnob(const std::string& name, Node* node, const std::string& description,int dimension, Knob_Mask flags);
    
};


/******************************KNOB_BASE**************************************/

class Knob : public QObject
{
    Q_OBJECT
    
public:
    enum Knob_Flags{NONE=0x0,INVISIBLE=0x1,READ_ONLY=0x2,RGBA_STYLE_SCALAR=0x4,NO_ALPHA=0x8};
    
    
    Knob(Node* node,const std::string& description,int dimension = 1,Knob_Mask flags = 0);
    
    virtual ~Knob();
    
    const std::string& getDescription() const { return _description; }
    
    const std::vector<U64>& getHashVector() const { return _hashVector; }
    
    Node* getNode() const { return _node; }
    
    int getDimension() const {return _dimension;}
            
    Knob_Mask getFlags() const;
    
    /*Must return the name of the knob. This name will be used by the KnobFactory
     to create an instance of this knob.*/
    virtual const std::string name()=0;
    
    virtual std::string serialize() const =0;
    
    void restoreFromString(const std::string& str){
        _restoreFromString(str);
        fillHashVector();
        emit valueChanged(_value);
        tryStartRendering();
    }
    
    template<typename T>
    void setValue(T variant[],int count){
        _value = Variant(variant,count);
        emit valueChanged(_value);
        tryStartRendering();
    }
    
    /*to allow implicit cast*/
    void setValue(const QVariant& variant){
        _value = variant;
        emit valueChanged(_value);
        tryStartRendering();
    }
    
    
    /*Set the value of the knob and emits the valueChanged signal*/
    void setValue(const Variant& variant){
        _value = variant;
        emit valueChanged(_value);
        tryStartRendering();
    }
    
    /*Used to extract the value held by the knob.
     Derived classes should provide a more appropriate
     way to retrieve results in the expected type.*/
    template<typename T>
    T value(){
        return _value.value<T>();
    }
    
    const Variant& getValueAsVariant() const { return _value; }
    
    /*You can call this when you want to remove this Knob
     at anytime.*/
    void deleteKnob();
    
    void turnOffNewLine();
    
    void setSpacingBetweenItems(int spacing);
    
    void setEnabled(bool b);
    
    void setVisible(bool b);
    
    void setMinimum(const QVariant& variant){
        _minimum = variant;
    }
    
    void setMinimum(const Variant& variant){
        _minimum = variant;
    }
    
    template<typename T>
    void setMinimum(T variant[],int count){
        _minimum = Variant(variant,count);
    }
    
    const Variant& getMinimum() const {return _minimum;}
    
    void setMaximum(const QVariant& variant){
        _maximum = variant;
    }
    
    void setMaximum(const Variant& variant){
        _maximum = variant;
    }
    
    template<typename T>
    void setMaximum(T variant[],int count){
        _maximum = Variant(variant,count);
    }
    
    const Variant& getMaximum() const {return _maximum;}

    
    void setIncrement(const QVariant& variant){
        _increment = variant;
    }
    void setIncrement(const Variant& variant){
        _increment = variant;
    }
    
    template<typename T>
    void setIncrement(T variant[],int count){
        _increment = Variant(variant,count);
    }
    
    const Variant& getIncrement() const {return _increment;}

    void setParentKnob(Knob* knob){_parentKnob = knob;}
    
    Knob* getParentKnob() const {return _parentKnob;}
    
    int determineHierarchySize() const;
    
public slots:
    /*Set the value of the knob but does NOT emit the valueChanged signal.
     This is called by the GUI*/
    void onValueChanged(const Variant& variant){
        _value = variant;
        fillHashVector();
        emit valueChangedByUser();
        tryStartRendering();
    }
    
signals:
    /*Emitted whenever the slot onValueChanged is called.
     This notifies listeners that the value held by the
     knob has changed by a user interaction.
     You can connect to this signal to do
     whatever processing you want.*/
    void valueChangedByUser();
    
    /*Emitted when the value is changed internally*/
    void valueChanged(const Variant&);
    
    /*emitted by deleteKnob()*/
    void deleteWanted();
   
    void enabledStateChanged(bool);
    
    void visibleStateChanged(bool);
    
protected:
    virtual void fillHashVector()=0; // function to add the specific values of the knob to the values vector.
    
    virtual void _restoreFromString(const std::string& str) =0;
    
    
    /*Should implement this to start rendering by calling startRendering.
     This is called by the onValueChanged() slot. If you don't want to
     start rendering, overload this function and make it do nothing.*/
    virtual void tryStartRendering() = 0;
    
    //Called to tell the engine to start processing
    //This is can be called in the implementation of tryStartRendering
    void startRendering(bool initViewer);
    
    Node *_node;
    Variant _value;
    std::vector<U64> _hashVector;
    
private:
    
    std::string _description;
    Knob_Mask _flags;
    int _dimension;
    
    bool _newLine;
    int _itemSpacing;
    
    /*Theses are used when it makes sense (e.g: for a scalar).
     They store exactly the same type as _value*/
    Variant _maximum,_minimum;
    Variant _increment;
    Knob* _parentKnob;
};

std::vector<Knob::Knob_Flags> Knob_Mask_to_Knobs_Flags(const Knob_Mask& m);

/******************************FILE_KNOB**************************************/

class File_Knob:public Knob
{
    Q_OBJECT
public:
    
    static Knob* BuildKnob(Node* node, const std::string& description,int dimension, Knob_Mask flags){
        return new File_Knob(node,description,dimension,flags);
    }
    
    File_Knob(Node* node, const std::string& description,int dimension, Knob_Mask flags=0):
    Knob(node,description,dimension,flags)
    {}
    
    virtual void fillHashVector(){
        // filenames of the sequence should not be involved in hash key computation as it defeats all the purpose of the cache.
        // explanation: The algorithm doing the look-up in the cache already takes care of the current frame name.
        // See  VideoEngine::render(...) at line starting with  key = FrameEntry::computeHashKey to understand the call.
    }
    
    virtual const std::string name(){return "InputFile";}
    
    virtual std::string serialize() const;
    
    void openFile(){
        emit shoudOpenFile();
    }
    
signals:
    void filesSelected();
    void shoudOpenFile();
    
protected:
    
    virtual void tryStartRendering();
    
    virtual void _restoreFromString(const std::string& str);
    
    
    
};

/******************************OUTPUT_FILE_KNOB**************************************/

class OutputFile_Knob:public Knob
{
    Q_OBJECT
public:
    
    static Knob* BuildKnob(Node* node, const std::string& description,int dimension, Knob_Mask flags){
        return new OutputFile_Knob(node,description,dimension,flags);
    }
    
    OutputFile_Knob(Node* node, const std::string& description,int dimension, Knob_Mask flags=0):
    Knob(node,description,dimension,flags)
    {}
    
    virtual void fillHashVector(){
        // filenames of the sequence should not be involved in hash key computation as it defeats all the purpose of the cache.
        // explanation: The algorithm doing the look-up in the cache already takes care of the current frame name.
        // See  VideoEngine::render(...) at line starting with  key = FrameEntry::computeHashKey to understand the call.
    }
    
    virtual const std::string name(){return "OutputFile";}
    
    virtual std::string serialize() const;
    
    void openFile(){
        emit shoudOpenFile();
    }

signals:
    
    void filesSelected();
    
    void shoudOpenFile();
    
protected:
    
    virtual void tryStartRendering();
    
    virtual void _restoreFromString(const std::string& str);
    
    
};

/******************************INT_KNOB**************************************/

class Int_Knob:public Knob
{
    
    
public:
    
    static Knob* BuildKnob(Node* node, const std::string& description,int dimension, Knob_Mask flags){
        return new Int_Knob(node,description,dimension,flags);
    }
    
    Int_Knob(Node* node, const std::string& description,int dimension, Knob_Mask flags=0):
    Knob(node,description,dimension,flags)
    {}
    
    virtual void fillHashVector();
    
    virtual const std::string name(){return "Int";}
    
    virtual std::string serialize() const;
    
    /*Returns a vector of values. The vector
     contains _dimension elements.*/
    const std::vector<int> getValues() const ;
    
protected:
    
    virtual void tryStartRendering();
    
    virtual void _restoreFromString(const std::string& str);
    
    
    
};

/******************************BOOL_KNOB**************************************/

class Bool_Knob:public Knob
{
    
public:
    
    static Knob* BuildKnob(Node* node, const std::string& description,int dimension, Knob_Mask flags){
        return new Bool_Knob(node,description,dimension,flags);
    }
    
    Bool_Knob(Node* node, const std::string& description,int dimension, Knob_Mask flags=0):
    Knob(node,description,dimension,flags)
    {}
    
    virtual void fillHashVector();
    
    virtual const std::string name(){return "Bool";}
    
    virtual std::string serialize() const;
    
    
    bool getValue() const { return _value.getQVariant().toBool(); }
    
protected:
    
    virtual void tryStartRendering();
    
    virtual void _restoreFromString(const std::string& str);
    
    
    
};

/******************************DOUBLE_KNOB**************************************/

class Double_Knob:public Knob
{
    
public:
    
    static Knob* BuildKnob(Node* node, const std::string& description,int dimension, Knob_Mask flags){
        return new Double_Knob(node,description,dimension,flags);
    }
    
    Double_Knob(Node* node, const std::string& description,int dimension, Knob_Mask flags=0):
    Knob(node,description,dimension,flags)
    {}
    
    virtual void fillHashVector();
    
    virtual const std::string name(){return "Double";}
    
    virtual std::string serialize() const;
    
    /*Returns a vector of values. The vector
     contains _dimension elements.*/
    const std::vector<double> getValues() const ;
    
protected:
    
    virtual void tryStartRendering();
    
    virtual void _restoreFromString(const std::string& str);
    
    
    
};

/******************************BUTTON_KNOB**************************************/

class Button_Knob:public Knob
{
    Q_OBJECT
    
public:
    
    static Knob* BuildKnob(Node* node, const std::string& description,int dimension, Knob_Mask flags){
        return new Button_Knob(node,description,dimension,flags);
    }
    
    Button_Knob(Node* node, const std::string& description,int dimension, Knob_Mask flags=0);
    
    virtual void fillHashVector(){}
    
    virtual const std::string name(){return "Button";}
    
    virtual std::string serialize() const{return "";}
    
    public slots:
    void connectToSlot(const Variant& v);
    
    
protected:
    
    virtual void tryStartRendering(){}
    
    virtual void _restoreFromString(const std::string& str){(void)str;}
    
    
    
};

/******************************COMBOBOX_KNOB**************************************/

class ComboBox_Knob:public Knob
{
    Q_OBJECT
public:
    
    static Knob* BuildKnob(Node* node, const std::string& description,int dimension, Knob_Mask flags){
        return new ComboBox_Knob(node,description,dimension,flags);
    }
    
    ComboBox_Knob(Node* node, const std::string& description,int dimension, Knob_Mask flags=0):
    Knob(node,description,dimension,flags)
    {
        
    }
    
    virtual void fillHashVector();
    
    virtual const std::string name(){return "ComboBox";}
    
    virtual std::string serialize() const;
    
    /*Must be called right away after the constructor.*/
    void populate(const QStringList& entries){
        _entries = entries;
        emit populated(_entries);
    }
    
signals:
    
    void populated(const QStringList&);
    
protected:
    
    virtual void tryStartRendering();
    
    virtual void _restoreFromString(const std::string& str);
    
    
private:
    QStringList _entries;
};

/******************************SEPARATOR_KNOB**************************************/

class Separator_Knob:public Knob
{
    
public:
    
    static Knob* BuildKnob(Node* node, const std::string& description,int dimension, Knob_Mask flags){
        return new Separator_Knob(node,description,dimension,flags);
    }
    
    Separator_Knob(Node* node, const std::string& description,int dimension, Knob_Mask flags=0):
    Knob(node,description,dimension,flags){
        
    }
    
    virtual void fillHashVector(){}
    
    virtual const std::string name(){return "Separator";}
    
    virtual std::string serialize() const{return "";}
    
protected:
    
    virtual void tryStartRendering(){}
    
    virtual void _restoreFromString(const std::string& str){(void)str;}
    
};
/******************************RGBA_KNOB**************************************/
class RGBA_Knob:public Knob
{
public:
    
    static Knob* BuildKnob(Node* node, const std::string& description,int dimension, Knob_Mask flags){
        return new RGBA_Knob(node,description,dimension,flags);
    }
    
    RGBA_Knob(Node* node, const std::string& description,int dimension, Knob_Mask flags=0):
    Knob(node,description,dimension,flags)
    {
        
    }
    
    virtual void fillHashVector();
    
    virtual const std::string name(){return "RGBA";}
    
    virtual std::string serialize() const;
    
    
protected:
    
    virtual void tryStartRendering(){
        startRendering(true);
    }
    
    virtual void _restoreFromString(const std::string& str);
    
    
private:
    QStringList _entries;
};

/******************************STRING_KNOB**************************************/
class String_Knob:public Knob
{
public:
    
    static Knob* BuildKnob(Node* node, const std::string& description,int dimension, Knob_Mask flags){
        return new String_Knob(node,description,dimension,flags);
    }
    
    String_Knob(Node* node, const std::string& description,int dimension, Knob_Mask flags=0):
    Knob(node,description,dimension,flags){
        
    }
    
    virtual void fillHashVector();
    
    virtual const std::string name(){return "String";}
    
    virtual std::string serialize() const;
    
protected:
    
    virtual void tryStartRendering(){
        startRendering(true);
    }
    
    virtual void _restoreFromString(const std::string& str);
    
    
private:
    QStringList _entries;
};
/******************************GROUP_KNOB**************************************/
class Group_Knob:public Knob
{
    Q_OBJECT
    
    std::vector<Knob*> _children;
public:
    
    static Knob* BuildKnob(Node* node, const std::string& description,int dimension, Knob_Mask flags){
        return new Group_Knob(node,description,dimension,flags);
    }
    
    Group_Knob(Node* node, const std::string& description,int dimension, Knob_Mask flags=0):
    Knob(node,description,dimension,flags)
    {
        
    }
    
    virtual void fillHashVector(){}
    
    virtual const std::string name(){return "Group";}
    
    virtual std::string serialize() const{return "";}
    
    void addKnob(Knob* k);
    
    const std::vector<Knob*>& getChildren() const {return _children;}
    
signals:
    void newChildAdded();
    
protected:
    
    virtual void tryStartRendering(){}
    
    virtual void _restoreFromString(const std::string& str){(void)str;}
    
};
/******************************TAB_KNOB**************************************/

class Tab_Knob:public Knob
{
    Q_OBJECT
public:
    
    static Knob* BuildKnob(Node* node, const std::string& description,int dimension, Knob_Mask flags){
        return new Tab_Knob(node,description,dimension,flags);
    }
    
    Tab_Knob(Node* node, const std::string& description,int dimension, Knob_Mask flags=0):
    Knob(node,description,dimension,flags){
        
    }
    
    virtual void fillHashVector(){}
    
    virtual const std::string name(){return "Tab";}
    
    virtual std::string serialize() const{return "";}
    
    void addTab(const std::string& name);
    
    void addKnob(const std::string& tabName,Knob* k);
    
signals:
    
    void newTabAdded(QString);
    void newKnobAdded();
    
protected:
    
    virtual void tryStartRendering(){}
    
    virtual void _restoreFromString(const std::string& str){(void)str;}
    
};

#endif // KNOB_H
