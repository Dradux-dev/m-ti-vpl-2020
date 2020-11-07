from colors.LinearGradient import LinearGradient


class LinearHorizontalGradient(LinearGradient):
    def __init__(self):
        LinearGradient.__init__(self, [(0, 0), (1, 0)], 2)

