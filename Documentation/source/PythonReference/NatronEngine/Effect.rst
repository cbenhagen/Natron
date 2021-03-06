.. module:: NatronEngine
.. _Effect:

Effect
******

**Inherits**: :doc:`Group` , :doc:`UserParamHolder`

Synopsis
--------

This object represents a single node in Natron, that is: an instance of a plug-in.
See :ref:`details`

Functions
^^^^^^^^^

*    def :meth:`endChanges<NatronEngine.Effect.endChanges>` ()
*    def :meth:`beginChanges<NatronEngine.Effect.beginChanges>` ()
*    def :meth:`canConnectInput<NatronEngine.Effect.canConnectInput>` (inputNumber, node)
*    def :meth:`connectInput<NatronEngine.Effect.connectInput>` (inputNumber, input)
*    def :meth:`createChild<NatronEngine.Effect.createChild>` ()
*    def :meth:`destroy<NatronEngine.Effect.destroy>` ([autoReconnect=true])
*    def :meth:`disconnectInput<NatronEngine.Effect.disconnectInput>` (inputNumber)
*    def :meth:`getColor<NatronEngine.Effect.getColor>` ()
*    def :meth:`getCurrentTime<NatronEngine.Effect.getCurrentTime>` ()
*    def :meth:`getInput<NatronEngine.Effect.getInput>` (inputNumber)
*    def :meth:`getLabel<NatronEngine.Effect.getLabel>` ()
*    def :meth:`getInputLabel<NatronEngine.Effect.getInputLabel>` (inputNumber)
*    def :meth:`getMaxInputCount<NatronEngine.Effect.getMaxInputCount>` ()
*    def :meth:`getParam<NatronEngine.Effect.getParam>` (name)
*    def :meth:`getParams<NatronEngine.Effect.getParams>` ()
*    def :meth:`getPluginID<NatronEngine.Effect.getPluginID>` ()
*    def :meth:`getPosition<NatronEngine.Effect.getPosition>` ()
*	 def :meth:`getRegionOfDefinition<NatronEngine.Effect.getRegionOfDefinition>` (time,view)
*    def :meth:`getRotoContext<NatronEngine.Effect.getRotoContext>` ()
*    def :meth:`getScriptName<NatronEngine.Effect.getScriptName>` ()
*    def :meth:`getSize<NatronEngine.Effect.getSize>` ()
*    def :meth:`getUserPageParam<NatronEngine.Effect.getUserPageParam>` ()
*    def :meth:`setColor<NatronEngine.Effect.setColor>` (r, g, b)
*    def :meth:`setLabel<NatronEngine.Effect.setLabel>` (name)
*    def :meth:`setPosition<NatronEngine.Effect.setPosition>` (x, y)
*    def :meth:`setScriptName<NatronEngine.Effect.setScriptName>` (scriptName)
*    def :meth:`setSize<NatronEngine.Effect.setSize>` (w, h)


.. _details:

Detailed Description
--------------------


The Effect object can be used to operate with a single node in Natron. 
To create a new Effect, use the :func:`app.createNode(pluginID)<NatronEngine.App.createNode>` function.
    
Natron automatically declares a variable to Python when a new Effect is created. 
This variable will have a script-name determined by Natron as explained in the 
:ref:`autovar` section.

Once an Effect is instantiated, it declares all its :doc:`Param` and inputs. 
See how to :ref:`manage <userParams>` user parameters below 

To get a specific :doc:`Param` by script-name, call the 
:func:`getParam(name) <NatronEngine.Effect.getParam>` function

Input effects are mapped against a zero-based index. To retrieve an input Effect
given an index, you can use the :func:`getInput(inputNumber) <NatronEngine.Effect.getInput>`
function. 
	
To manage inputs, you can connect them and disconnect them with respect to their input
index with the :func:`connectInput(inputNumber,input)<NatronEngine.Effect.connectInput>` and
then :func:`disconnectInput(inputNumber)<NatronEngine.Effect.disconnectInput>` functions.

If you need to destroy permanently the Effect, just call :func:`destroy() <NatronEngine.Effect.destroy()>`.

For convenience some GUI functionalities have been made accessible via the Effect class
to control the GUI of the node (on the node graph):
	
	* Get/Set the node position with the :func:`setPosition(x,y)<NatronEngine.Effect.setPosition>` and :func:`getPosition()<NatronEngine.Effect.getPosition>` functions
	* Get/Set the node size with the :func:`setSize(width,height)<NatronEngine.Effect.setSize>` and :func:`getSize()<NatronEngine.Effect.getSize>` functions
	* Get/Set the node color with the :func:`setColor(r,g,b)<NatronEngine.Effect.setColor>` and :func:`getColor()<NatronEngine.Effect.getColor>` functions
	
.. _userParams:

Creating user parameters
^^^^^^^^^^^^^^^^^^^^^^^^

See :ref:`this section<userParams.details>`

Member functions description
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. method:: NatronEngine.Effect.beginChanges()

	Starts a begin/End bracket, blocking all evaluation (=renders and callback onParamChanged) that would be issued due to
	a call to :func:`setValue<NatronEngine.IntParam.setValue>` on any parameter of the Effect.
	
	Typically to change several values at once we bracket the changes like this::
	
		node.beginChanges()	
		param1.setValue(...)
		param2.setValue(...)
		param3.setValue(...)
		param4.setValue(...)
		node.endChanges()  # This triggers a new render and a call to the onParamChanged callback




.. method:: NatronEngine.Effect.endChanges()

	Ends a begin/end bracket. If the begin/end bracket recursion reaches 0 and there were calls
	made to :func:`setValue<NatronEngine.IntParam.setValue>` this function will effectively compresss
	all evaluations into a single one.
	See :func:`beginChanges()<NatronEngine.Effect.beginChanges>`





.. method:: NatronEngine.Effect.canConnectInput(inputNumber, node)


    :param inputNumber: :class:`int<PySide.QtCore.int>`
    :param node: :class:`Effect<NatronEngine.Effect>`
    :rtype: :class:`bool<PySide.QtCore.bool>`


Returns whether the given *node* can be connected at the given *inputNumber* of this
Effect. This function could return False for one of the following reasons:
	
	* The Effect already has an input at the given *inputNumber*
	* The *node* is None
	* The given *inputNumber* is out of range
	* The *node* cannot have any node connected to it (such as a BackDrop or an Output)
	* This Effect or the given *node* is a child of another node (for trackers only)
	* Connecting *node* would create a cycle in the graph implying that it would create infinite recursions
	* The pixel aspect ratio of *node* is different than one of the already existing input of this Effect and this Effect cannot handle different input pixel aspect ratios
	* The frame rate of *node* is different than one of the already existing input


.. method:: NatronEngine.Effect.connectInput(inputNumber, input)


    :param inputNumber: :class:`int<PySide.QtCore.int>`
    :param input: :class:`Effect<NatronEngine.Effect>`
    :rtype: :class:`bool<PySide.QtCore.bool>`
	
Connects *input* to the given *inputNumber* of this Effect. 
This function calls internally :func:`canConnectInput()<NatronEngine.Effect.canConnectInput>`
to determine if a connection is possible. 



.. method:: NatronEngine.Effect.createChild()


    :rtype: :class:`Effect<NatronEngine.Effect>`

If this Effect is a multi-instance node (currently only the Tracker node is a multi-instance)
then this function will create a new instance of the same plug-in as a child of this node.
This is used for Trackers to create new tracks: internally each track is in fact a 
separate node on its own.




.. method:: NatronEngine.Effect.destroy([autoReconnect=true])


    :param autoReconnect: :class:`bool<PySide.QtCore.bool>`
		
Removes this Effect from the current project definitively.
If *autoReconnect* is True then any nodes connected to this node will try to connect
their input to the input of this node instead.
	


.. method:: NatronEngine.Effect.disconnectInput(inputNumber)


    :param inputNumber: :class:`int<PySide.QtCore.int>`

Removes any input Effect connected to the given *inputNumber* of this node.




.. method:: NatronEngine.Effect.getColor()

	:rtype: :class:`tuple`
	
Returns the color of this node as it appears on the node graph as [R,G,B] 3-dimensional tuple.





.. method:: NatronEngine.Effect.getCurrentTime()


    :rtype: :class:`int<PySide.QtCore.int>`


Returns the current time of timeline if this node is currently rendering, otherwise
it returns the current time at which the node is currently rendering for the caller
thread.



.. method:: NatronEngine.Effect.getInput(inputNumber)


    :param inputNumber: :class:`int<PySide.QtCore.int>`
    :rtype: :class:`Effect<NatronEngine.Effect>`


    
Returns the node connected at the given *inputNumber*.
    



.. method:: NatronEngine.Effect.getLabel()


    :rtype: :class:`str<NatronEngine.std::string>`

Returns the *label* of the node. See :ref:`this section<autoVar>` for a discussion
of the *label* vs the *script-name*.

.. method:: NatronEngine.Effect.getInputLabel(inputNumber)


	:param inputNumber: :class:`int<PySide.QtCore.int>`
    :rtype: :class:`str<NatronEngine.std::string>`

Returns the label of the input at the given *inputNumber*.
It corresponds to the label displayed on the arrow of the input in the node graph.

.. method:: NatronEngine.Effect.getMaxInputCount()


    :rtype: :class:`int<PySide.QtCore.int>`

Returns the number of inputs for the node. Graphically this corresponds to the number
of arrows in input.




.. method:: NatronEngine.Effect.getParam(name)


    :param name: :class:`str<NatronEngine.std::string>`
    :rtype: :class:`Param<Param>`


Returns a :doc:`parameter<Param>` by its script-name or None if
no such parameter exists.



.. method:: NatronEngine.Effect.getParams()


    :rtype: :class:`sequence`

Returns all the :doc:`parameters<NatronEngine.Param>` of this Effect as a sequence.
	



.. method:: NatronEngine.Effect.getPluginID()


    :rtype: :class:`str<NatronEngine.std::string>`


Returns the ID of the plug-in that this node instantiate.



.. method:: NatronEngine.Effect.getPosition()


	:rtype: :class:`tuple`

Returns the current position of the node on the node-graph. This is a 2
dimensional [X,Y] tuple.
Note that in background mode, if used, this function will always return [0,0] and
should NOT be used.


.. method:: NatronEngine.Effect.getRegionOfDefinition(time,view)

	:param time: :class:`int`
	:param view: :class:`view`
	:rtype: :class:`PySide.QtCore.QRectF`
	
Returns the bounding box of the image produced by this effect in canonical coordinates. 
This is exactly the value displayed in the "Info" tab of the settings panel of the node
for the "Output".
This can be useful for example to set the position of a point parameter to the center
of the region of definition.

.. method:: NatronEngine.Effect.getRotoContext()


    :rtype: :class:`Roto<NatronEngine.Roto>`

Returns the roto context for this node. Currently only the Roto node has a roto context.
The roto context is in charge of maintaining all informations relative to :doc:`Beziers<BezierCurve>`
and :doc:`Layers<Layer>`.
Most of the nodes don't have a roto context though and this function will return None.



.. method:: NatronEngine.Effect.getScriptName()


    :rtype: :class:`str<NatronEngine.std::string>`


Returns the script-name of this Effect. See :ref:`this<autoVar>` section for more
information about the script-name.



.. method:: NatronEngine.Effect.getSize()

	:rtype: :class:`tuple`

Returns the size of this node on the node-graph as a 2 dimensional [Width,Height] tuple.
Note that calling this function will in background mode will always return [0,0] and
should not be used.





.. method:: NatronEngine.Effect.getUserPageParam()


    :rtype: :class:`PageParam<NatronEngine.PageParam>`


Convenience function to return the user page parameter if this Effect has one.



.. method:: NatronEngine.Effect.setColor(r, g, b)


    :param r: :class:`float<PySide.QtCore.double>`
    :param g: :class:`float<PySide.QtCore.double>`
    :param b: :class:`float<PySide.QtCore.double>`
	
Set the color of the node as it appears on the node graph.
Note that calling this function will in background mode will do nothing and
should not be used.



.. method:: NatronEngine.Effect.setLabel(name)


    :param name: :class:`str<NatronEngine.std::string>`

Set the label of the node as it appears in the user interface.
See :ref:`this<autoVar>` section for an explanation of the difference between the *label* and the
*script-name*.



.. method:: NatronEngine.Effect.setPosition(x, y)


    :param x: :class:`float<PySide.QtCore.double>`
    :param y: :class:`float<PySide.QtCore.double>`


Set the position of the node as it appears on the node graph.
Note that calling this function will in background mode will do nothing and
should not be used.



.. method:: NatronEngine.Effect.setScriptName(scriptName)


    :param scriptName: :class:`str<NatronEngine.std::string>`
    :rtype: :class:`bool<PySide.QtCore.bool>`

Set the script-name of the node as used internally by Natron.
See :ref:`this<autoVar>` section for an explanation of the difference between the *label* and the
*script-name*.

.. warning::

    Using this function will remove any previous variable declared using the
    old script-name and will create a new variable with the new script name if valid.
	
If your script was using for instance a node named::
	
    app1.Blur1
		
and you renamed it BlurOne, it should now be available to Python this way::
	
    app1.BlurOne
		
but using app1.Blur1 would report the following error::
	
    Traceback (most recent call last):
    File "<stdin>", line 1, in <module>
    NameError: name 'Blur1' is not defined




.. method:: NatronEngine.Effect.setSize(w, h)


    :param w: :class:`float<PySide.QtCore.double>`
    :param h: :class:`float<PySide.QtCore.double>`

Set the size of the node as it appears on the node graph.
Note that calling this function will in background mode will do nothing and
should not be used.





