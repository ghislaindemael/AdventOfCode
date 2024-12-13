class GardenPlot:

    def __init__(self, newCrop):
        self.crop = newCrop
        self.regionID = None
        self.x = -1
        self.y = -1

    def setCoord(self, x, y):
        self.x = x
        self.y = y

    def getCrop(self):
        return self.crop

    def sameCrop(self, plot):
        return self.crop == plot.getCrop()

    def isRegionSet(self):
        return self.regionID is not None

    def getRegionID(self):
        return self.regionID

    def setRegionID(self, id):
        self.regionID = id

    def sameRegion(self, region):
        return self.regionID == region.id

    def countFences(self, field, x, y):
        fence_count = 0
        for i in range(4):
            if self.hasFenceAtSide(field, i):
                fence_count += 1
        return fence_count

    def getNeighbour(self, field, direction):
        if direction == 0:
            if self.x == 0:
                return None
            return field[self.x - 1][self.y]
        elif direction == 1:
            if self.y == len(field) - 1:
                return None
            return field[self.x][self.y + 1]
        elif direction == 2:
            if self.x == len(field) - 1:
                return None
            return field[self.x + 1][self.y]
        elif direction == 3:
            if self.y == 0:
                return None
            return field[self.x][self.y - 1]

    def getTopNeighbour(self, field):
        return self.getNeighbour(field, 0)

    def getRightNeighbour(self, field):
        return self.getNeighbour(field, 1)

    def getBottomNeighbour(self, field):
        return self.getNeighbour(field, 2)

    def getLeftNeighbour(self, field):
        return self.getNeighbour(field, 3)

    def hasFenceAtSide(self, field, side):
        if side == 0:
            if self.x == 0:
                return True
            return self.getTopNeighbour(field).getCrop() != self.crop
        elif side == 1:
            if self.y == len(field) - 1:
                return True
            return self.getRightNeighbour(field).getCrop() != self.crop
        elif side == 2:
            if self.x == len(field) - 1:
                return True
            return self.getBottomNeighbour(field).getCrop() != self.crop
        else:
            if self.y == 0:
                return True
            return self.getLeftNeighbour(field).getCrop() != self.crop

    def __str__(self):
        return self.crop + "(" + str(self.regionID) + ")"

    def __repr__(self):
        return self.__str__()


class Region:
    def __init__(self, region_id):
        self.id = region_id
        self.plotCount = 0
        self.sides = 0

    def addPlot(self):
        self.plotCount += 1

    def addSide(self):
        self.sides += 1

    def getSides(self):
        return self.sides


def propagateRegionID(field, plot, nextPlot, region):
    if nextPlot is None or nextPlot.isRegionSet():
        return

    if plot.sameCrop(nextPlot):
        nextPlot.setRegionID(region.id)
        region.addPlot()
        propagateRegionID(field, plot, nextPlot.getTopNeighbour(field), region)
        propagateRegionID(field, plot, nextPlot.getBottomNeighbour(field), region)
        propagateRegionID(field, plot, nextPlot.getLeftNeighbour(field), region)
        propagateRegionID(field, plot, nextPlot.getRightNeighbour(field), region)


def addFences(field, x, y):
    plot = field[x][y]
    fence_count = plot.countFences(field, x, y)
    return fence_count


def main():
    field = []
    with open("input.txt", 'r') as file:
        for line in file:
            row = []
            for crop in line:
                if crop.isalpha():
                    plot = GardenPlot(crop)
                    plot.setCoord(len(field), len(row))
                    row.append(plot)
            field.append(row)

    regions = []
    for i in range(len(field)):
        for j in range(len(field[i])):
            plot = field[i][j]
            if not plot.isRegionSet():
                newRegion = Region(len(regions))
                plot.setRegionID(newRegion.id)
                newRegion.addPlot()
                regions.append(newRegion)

                propagateRegionID(field, plot, plot.getTopNeighbour(field), newRegion)
                propagateRegionID(field, plot, plot.getRightNeighbour(field), newRegion)
                propagateRegionID(field, plot, plot.getBottomNeighbour(field), newRegion)
                propagateRegionID(field, plot, plot.getLeftNeighbour(field), newRegion)

    # print(field)

    totalFencesPrice = 0
    bulkFencesPrice = 0

    for region in regions:
        region_fence_count = 0
        for i, row in enumerate(field):
            for j, plot in enumerate(row):
                if plot.sameRegion(region):
                    region_fence_count += addFences(field, i, j)
                    for side in [0, 1, 2, 3]:
                        if plot.hasFenceAtSide(field, side):
                            if side % 2 == 0:
                                neighbor = plot.getLeftNeighbour(field)
                                if plot.y == 0 or not plot.sameCrop(neighbor) or (
                                        plot.sameCrop(neighbor) and not neighbor.hasFenceAtSide(field, side)):
                                    region.addSide()
                            else:
                                neighbor = plot.getTopNeighbour(field)
                                if plot.x == 0 or not plot.sameCrop(neighbor) or (
                                        plot.sameCrop(neighbor) and not neighbor.hasFenceAtSide(field, side)):
                                    region.addSide()

        totalFencesPrice += region_fence_count * region.plotCount
        bulkFencesPrice += region.sides * region.plotCount

    print(f"Total Fences price: {totalFencesPrice}")
    print(f"Bulk fences price: {bulkFencesPrice}")


if __name__ == '__main__':
    main()
