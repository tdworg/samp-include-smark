/** ======================================================================== **\
	Benchmark

| Description:

| Version:
	1.0.0

| Dependencies:
	- n\a

| Compatible with the JIT plugin:
	Yes

| License:
	Copyright (c) 2017 Double V

	This software is provided 'as-is', without any express or implied
	warranty. In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1.	The origin of this software must not be misrepresented; you must not
		claim that you wrote the original software. If you use this software
		in a product, an acknowledgement in the product documentation would be
		appreciated but is not required.
	2.	Altered source versions must be plainly marked as such, and must not be
		misrepresented as being the original software.
	3.	This notice may not be removed or altered from any source distribution.
\** ======================================================================== **/

#if defined _inc_tdw_benchmark
	#undef _inc_tdw_benchmark
#endif

#if defined BENCHMARK_NEW_INIT
	#if BENCHMARK_NEW_INIT
		#undef BENCHMARK_NEW_INIT
		#undef BENCHMARK_MAJOR
		#undef BENCHMARK_MINOR
	#endif
#endif

#if defined _TDW_BENCHMARK
	#endinput
#endif
#define _TDW_BENCHMARK

#define _BENCHMARK_DEFAULT_MAJOR (1_000)
#define _BENCHMARK_DEFAULT_MINOR (10_000)
#define _BENCHMARK_MAX_ITEM_NAME (31)
#define _BENCHMARK_MAX_ITEMS     (16)

#if !defined UNTAG
	#define UNTAG(%1) _:(%1)
#endif

#define BENCHMARK_INIT(%1,%2,%3) const BENCHMARK_MAJOR=UNTAG(%1),BENCHMARK_MINOR=UNTAG(%2),bool:BENCHMARK_NEW_INIT=(%3)

#define BENCHMARK_MAJOR=UNTAG(-1) BENCHMARK_MAJOR=UNTAG(_BENCHMARK_DEFAULT_MAJOR)
#define BENCHMARK_MINOR=UNTAG(-1) BENCHMARK_MINOR=UNTAG(_BENCHMARK_DEFAULT_MINOR)

#define BENCHMARK_GROUP(%1)\
	for (new bool:bml@once=benchmark_group_init(), bml@i, bml@t, bml@c, bml@mj, bml@mi = _:BENCHMARK_CHECK_VARS:0, %1;\
		bml@once;\
		benchmark_group_end(bml@c, BENCHMARK_MAJOR, BENCHMARK_MINOR), bml@once=false)\
		for( ; bml@mj < BENCHMARK_MAJOR; ++bml@mj, bml@i = 0)
#define BENCHMARK_CHECK_VARS:%0,; %0;
#define bml@%0\32; bml@%0

#define BENCHMARK_ITEM(%1)\
	for (bml@once = benchmark_item_init(#%1, bml@mj, bml@i, bml@c);\
		bml@once;\
		benchmark_item_end(GetTickCount()-bml@t, bml@i), bml@once=false)\
		for(bml@mi=0,bml@t=GetTickCount(); bml@mi < BENCHMARK_MINOR; ++bml@mi)

enum BenchmarkGroup {
	EI_BM_NAME[_BENCHMARK_MAX_ITEM_NAME],
	EI_BM_AVERAGE_TIME
};

static stock const
	TDW_g_sNullGroup[BenchmarkGroup];

static stock
	TDW_g_sStaticMsg = 0;

stock
	TDW_g_sGroup[_BENCHMARK_MAX_ITEMS][BenchmarkGroup];

forward bool:benchmark_group_init();
forward bool:benchmark_item_init(const name[], major_it, index, &count);

static stock benchmark_show_static_msg()
{
	print(!" |==============================================|");
	print(!" | Benchmarks                                   |");
	print(!" |==============================================|");
}

stock bool:benchmark_group_init()
{
	if (_:TDW_g_sStaticMsg == 0) {
		benchmark_show_static_msg();
		TDW_g_sStaticMsg = 1;
	}

	for (new i=0; i < _BENCHMARK_MAX_ITEMS; ++i)
		TDW_g_sGroup[i] = TDW_g_sNullGroup;

	print(!" |                                              |");
	print(!" |-----               Start                -----|");
	print(!" |                                              |");

	return true;
}

stock bool:benchmark_item_init(const name[], major_it, index, &count)
{
	if (index >= _BENCHMARK_MAX_ITEMS)
		return false;

	if (major_it == 0) {
		TDW_g_sGroup[index][EI_BM_NAME] = EOS;
		strcat(TDW_g_sGroup[index][EI_BM_NAME], name);
		count++;
	}

	return true;
}

stock benchmark_item_end(diff, &index)
{
	TDW_g_sGroup[index++][EI_BM_AVERAGE_TIME] += diff;
}

stock benchmark_group_end(count, major_it, minor_it)
{
	#pragma unused major_it, minor_it

	new fastid, lasttime = TDW_g_sGroup[0][EI_BM_AVERAGE_TIME];

	print(!" | Name                             | Time (ms) |");
	print(!" |----------------------------------|-----------|");

	for (new i = 0; i != count; i++)
	{
		if (TDW_g_sGroup[i][EI_BM_AVERAGE_TIME] < lasttime) {
			lasttime = TDW_g_sGroup[i][EI_BM_AVERAGE_TIME];
			fastid = i;
		}
	}

	for (new i = 0; i != count; i++)
	{
		printf(" | %c%31s | %9d |", (i == fastid ? '*' : ' '),
			TDW_g_sGroup[i][EI_BM_NAME], TDW_g_sGroup[i][EI_BM_AVERAGE_TIME]);
	}

	print(!" |----------------------------------|-----------|");
	printf(" | %2d parts                                     |", count);
	print(!" |                                              |");
	print(!" |-----                End                ------|");
	print(!" |                                              |");
}
