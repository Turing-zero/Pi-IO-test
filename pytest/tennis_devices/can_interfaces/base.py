class CANInterfaceBase:
    """
    Base class for CAN interfaces.
    This class defines the basic structure and methods that any CAN interface should implement.
    """

    def __init__(self, name: str):
        self.name = name

    def open(self):
        """Open the CAN interface."""
        raise NotImplementedError("This method should be overridden by subclasses.")

    def close(self):
        """Close the CAN interface."""
        raise NotImplementedError("This method should be overridden by subclasses.")

    def send(self, _id, _data):
        """
        Send a message over the CAN interface.
        
        Parameters:
        _id (int): The CAN ID of the message.
        _data (list): The data to be sent in the message.
        """
        raise NotImplementedError("This method should be overridden by subclasses.")

    def receive(self, timeout=None):
        """
        Receive a message from the CAN interface.
        
        Parameters:
        timeout (float): The maximum time to wait for a message.
        
        Returns:
        tuple: A tuple containing the CAN ID and data received.
        """
        raise NotImplementedError("This method should be overridden by subclasses.")