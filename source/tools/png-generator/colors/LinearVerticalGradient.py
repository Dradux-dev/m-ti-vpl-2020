from colors.LinearGradient import LinearGradient


class LinearVerticalGradient(LinearGradient):
    def __init__(self):
        LinearGradient.__init__(self, [(0, 0), (0, 1)], 2)
