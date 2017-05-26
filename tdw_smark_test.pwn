#include <a_samp>

#include "tdw_smark\tdw_smark.inc"

BENCHMARK_INIT(-1, -1, true);

main()
{
	DISABLE_BENCHMARK_GROUP(abc[100])
	{
		BENCHMARK_ITEM(format)
		{
			format(abc, sizeof abc, "locallocallocallocallocallocallocallocallocal");
		}

		BENCHMARK_ITEM(strcat)
		{
			abc[0] = 0;
			strcat(abc, "locallocallocallocallocallocallocallocallocal");
		}
	}

	BENCHMARK_GROUP(abc[100])
	{
		BENCHMARK_ITEM(format)
		{
			format(abc, sizeof abc, "locallocallocallocallocallocallocallocallocal");
		}

		BENCHMARK_ITEM(strcat)
		{
			abc[0] = 0;
			strcat(abc, "locallocallocallocallocallocallocallocallocal");
		}
	}
}
