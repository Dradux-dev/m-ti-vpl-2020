class Form:
    """
    @brief Form

    baseclass for all forms

    @author Philipp Koopke
    """

    def __init__(self, size=40):
        """
        @brief Constructor

        Constructs the form with a size of 40.

        @param size the size of the from as int.

        @author Philipp Koopke
        """
        self.__objWidth = size
        self.__objHeight = size
        self.__enabled = True
        self.__renderBoundingBox = False

    @property
    def getWidth(self):
        """
        @brief getWidth

        return the width of the form.
        
        @author Philipp Koopke
        """
        return self.__objWidth

    @property
    def getHeight(self):
        """
        @brief getHeight

        return the height of the form.
        
        @author Philipp Koopke
        """
        return self.__objHeight

    def setSize(self, size):
        """
        @brief setSize

        Adjusts the size of the form.
        
        @param size the new size of the form as int.

        @author Philipp Koopke
        """
        self.__objWidth = size
        self.__objHeight = size

    def enable(self):
        """
        @brief enable

        enable the form
        
        @author Philipp Koopke
        """
        self.__enabled = True

    def disable(self):
        """
        @brief disable

        disable the form
        
        @author Philipp Koopke
        """
        self.__enabled = False

    def isEnabled(self):
        """
        @brief isEnabled

        return the enavle status of the form.
        
        @author Philipp Koopke
        """
        return self.__enabled

    @property
    def renderBoundingBox(self):
        """
        @brief isEnabled

        return the status if the BoundingBox will rendered
        
        @author Philipp Koopke
        """
        return self.__renderBoundingBox

    @renderBoundingBox.setter
    def renderBoundingBox(self, value):
        """
        @brief isEnabled

	set the status if the BoundingBox will rendered

        @param value ist the new status if the BoundingBox will rendered
        
        @author Philipp Koopke
        """
        self.__renderBoundingBox = value

