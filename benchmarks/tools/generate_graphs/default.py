from typing import List
from benchmarks import source_info

default_sources: List[source_info] = [
	source_info(
		"libc++.benchmarks.json",
		"libc++",
		False,
	),
	source_info(
		"libstdc++.benchmarks.json",
		"libstdc++",
		False,
	),
	source_info(
		"vc++.benchmarks.json",
		"vc++",
		False,
	)
]
