#include "EntityTester.h"



EntityTester::EntityTester(uint32_t numOfComponent, uint32_t maxEntityNumber):mNoOfComponents(numOfComponent), mMaxEntityNumber(maxEntityNumber),
mVec3SparseSet(maxEntityNumber), mIntSparseSet(maxEntityNumber), mVec3SparseSetMS(maxEntityNumber),
mVec3SparseSetRO(maxEntityNumber), mIntSparseSetMS(maxEntityNumber), mIntSparseSetRO(maxEntityNumber)
{
	resizeTestVectors(mNoOfComponents);
	mEntityOffset = (mMaxEntityNumber) / numOfComponent;
	randomFillData();
	mostlySortedFillData(15);

}


EntityTester::~EntityTester()
{
}

void EntityTester::resizeTestVectors(uint32_t size)
{
	mVec3Baseline.resize(size);
	mIntBaseline.resize(size);
	mIntMostlySorted.resize(size);
	mIntReverseOrder.resize(size);
	mVec3MostlySorted.resize(size);
	mVec3ReverseOrder.resize(size);
}
void EntityTester::randomFillData()
{
	std::default_random_engine RNG;
	std::uniform_real_distribution<float> floatDistribution(-100.0f,100.0f);
	std::uniform_int_distribution<uint32_t> intDistribution(0, 1000);
	for (int i = 1; i < mNoOfComponents; i++)
	{
		mVec3Baseline[i].x =floatDistribution(RNG);
		mVec3Baseline[i].y = floatDistribution(RNG);
		mVec3Baseline[i].z = floatDistribution(RNG);

		mIntBaseline[i] = intDistribution(RNG);

	}

	for(int i = 1; i < mNoOfComponents; ++i)
	{
	//	std::cout << "\n" << i;
		mVec3SparseSet.addComponent(i * mEntityOffset,mVec3Baseline[i]);
		mIntSparseSet.addComponent(i * mEntityOffset, mIntBaseline[i]);
	}
}

void EntityTester::mostlySortedFillData(int numOfUnsorted)
{
	std::vector<int> unsortedElements(numOfUnsorted);
	std::default_random_engine RNG;
	std::uniform_real_distribution<float> floatDistribution(1, mNoOfComponents);
	std::uniform_int_distribution<uint32_t> intDistribution(1, mNoOfComponents);
	for (int i = 0; i < numOfUnsorted; ++i)
	{
		unsortedElements[i] = intDistribution(RNG);
	}
	//create sorted SS and Baseline Vectors
	for (int i = 1; i < mNoOfComponents; ++i)
	{
		mVec3MostlySorted[i].x = i;
		mVec3MostlySorted[i].y = i;
		mVec3MostlySorted[i].z = i;

		mIntMostlySorted[i] = i;

		mVec3SparseSetMS.addComponent(i*mEntityOffset, mVec3MostlySorted[i]);
		mIntSparseSetMS.addComponent(i*mEntityOffset, mIntMostlySorted[i]);
	}
	//add unsorted elements;
	for (int i = 0; i < numOfUnsorted; ++i)
	{
		mVec3MostlySorted[unsortedElements[i]] = vec3(floatDistribution(RNG), floatDistribution(RNG), floatDistribution(RNG));
		mIntMostlySorted[unsortedElements[i]] = intDistribution(RNG);

		mVec3SparseSetMS[unsortedElements[i] *mEntityOffset] = mVec3MostlySorted[i];
		mIntSparseSetMS[unsortedElements[i] *mEntityOffset] = mIntMostlySorted[i];
	}
}
void EntityTester::sortData()
{
	std::cout << "\n -----Sorting " << mNoOfComponents << " elements----- \n\n";
	insertionSort();

	stdSort();

	swapSort();
}
void EntityTester::insertionSort()
{
	SparseSet<uint32_t> sparseRandInt = mIntSparseSet;
	SparseSet<vec3> sparseRandVec3 = mVec3SparseSet;
	SparseSet<uint32_t> sparseMSInt = mIntSparseSetMS;
	SparseSet<vec3> sparseMSVec3 = mVec3SparseSetMS;

	mTimer.startTimer();
	sparseRandInt.InsertSort(1,mNoOfComponents);
	mTimer.printTime("\n Insertion sort on sparse set with random ints: ");


	mTimer.startTimer();
	sparseRandVec3.InsertSort(1, mNoOfComponents);
	mTimer.printTime("\n Insertion sort on sparse set with random vec3s: ");

	mTimer.startTimer();
	sparseMSInt.InsertSort(1, mNoOfComponents);
	mTimer.printTime("\n Insertion sort on sparse set with partially sorted ints: ");


	mTimer.startTimer();
	sparseMSVec3.InsertSort(1, mNoOfComponents);
	mTimer.printTime("\n Insertion sort on sparse set with partially sorted vec3s: ");

//	for (int i = 1; i < 100; i++)
//	{
//		std::cout <<"\n" <<i<<" "<<sparseRandVec3[i*mEntityOffset].distance();
//	}
}

void EntityTester::stdSort()
{
	std::vector<vec3> stdSortVec3Baseline = mVec3Baseline;
	std::vector<vec3> stdSortIntBaseline = mVec3Baseline;

	mTimer.startTimer();
	std::sort(stdSortVec3Baseline.begin(), stdSortVec3Baseline.end());
	mTimer.printTime("\n std sort on vector with random sorted vec3s: ");

	mTimer.startTimer();
	std::sort(stdSortIntBaseline.begin(), stdSortIntBaseline.end());
	mTimer.printTime("\n std sort on vector with random sorted ints: ");

}

void EntityTester::swapSort()
{
	SparseSet<uint32_t> sparseRandInt = mIntSparseSet;
	SparseSet<vec3> sparseRandVec3 = mVec3SparseSet;
	SparseSet<uint32_t> sparseMSInt = mIntSparseSetMS;
	SparseSet<vec3> sparseMSVec3 = mVec3SparseSetMS;

	mTimer.startTimer();
	sparseRandInt.quickSort(1, mNoOfComponents);
	mTimer.printTime("\n EntitySwapSort sort on sparse set with random ints: ");
	for (int i = 9975; i<mNoOfComponents; i++)
	{
		std::cout<<"\n"<<sparseRandInt(i);
	}

	mTimer.startTimer();
	sparseRandVec3.quickSort(1, mNoOfComponents);
	mTimer.printTime("\n EntitySwapSort sort on sparse set with random vec3s: ");
	for (int i = 9975; i < mNoOfComponents; i++)
	{
		std::cout << "\n" << sparseRandVec3(i).distance();
	}


	mTimer.startTimer();
	sparseMSInt.quickSort(1, mNoOfComponents);
	mTimer.printTime("\n EntitySwapSort sort on sparse set with partially sorted ints: ");
	for (int i = 9975; i < mNoOfComponents; i++)
	{
		std::cout << "\n" << sparseMSInt(i);
	}

	mTimer.startTimer();
	sparseMSVec3.quickSort(1, mNoOfComponents);
	mTimer.printTime("\n EntitySwapSort sort on sparse set with partially sorted vec3s: ");
	for (int i = 9975; i < mNoOfComponents; i++)
	{
		std::cout << "\n" << sparseMSVec3(i).distance();
	}
	//255th entity before sorting, all entity values should be the same
	std::cout << "\n\n\n" << mVec3SparseSet[255 * mEntityOffset].distance();
	//255th element of dense set
	std::cout << "\n\n\n 255th element of dense set " << sparseRandVec3(255).distance();
	//last element of dense set
	std::cout << "\n\n\n last element of dense set " << sparseRandVec3(mNoOfComponents - 1).distance();
	//255th entity after sorting
	std::cout << "\n\n\n 255th entity after sorting " << sparseRandVec3[(255) * mEntityOffset].distance();
	sparseRandVec3.swapInsert(mNoOfComponents-1, 255);
	std::cout << "\n\n\n 255th element of dense set after swapping " << sparseRandVec3(255).distance();
	//last element after swapping
	std::cout << "\n\n\n last element of dense set after swapping " << sparseRandVec3(mNoOfComponents-1).distance();
	//255th entity after swapping
	std::cout << "\n\n\n 255th entity after swapping " << sparseRandVec3[(255) * mEntityOffset].distance();

}

void EntityTester::removeData()
{
	SparseSet<uint32_t> sparseRandInt = mIntSparseSet;
	SparseSet<vec3> sparseRandVec3 = mVec3SparseSet;
	SparseSet<uint32_t> sparseMSInt = mIntSparseSetMS;
	SparseSet<vec3> sparseMSVec3 = mVec3SparseSetMS;
	sparseMSInt.quickSort(1, mNoOfComponents);
	sparseMSVec3.quickSort(1, mNoOfComponents);


	mTimer.startTimer();
	for (int i = 1; i < mNoOfComponents; i += 2)
	{
		sparseMSVec3.deleteSortedComponent(i*mEntityOffset);
	}
	mTimer.printTime(std::string("\n\n Deleting 5000 sorted vec3s "));

	mTimer.startTimer();
	for (int i = 1; i < mNoOfComponents; i += 2)
	{
		sparseMSInt.deleteSortedComponent(i*mEntityOffset);
	}
	mTimer.printTime(std::string("\n\n Deleting 5000 sorted ints "));

	mTimer.startTimer();
	for (int i = 1; i < mNoOfComponents; i += 2)
	{
		sparseRandVec3.deleteUnsortedComponent(i*mEntityOffset);
	}
	mTimer.printTime(std::string("\n\n Deleting 5000 unsorted vec3s "));

	mTimer.startTimer();
	for (int i = 1; i < mNoOfComponents; i += 2)
	{
		sparseRandInt.deleteUnsortedComponent(i*mEntityOffset);
	}
	mTimer.printTime(std::string("\n\n Deleting 5000 unsorted ints "));

}