#pragma once

#ifdef RNET_DEBUG
	#define RNET_ENABLE_LOGS
	#define RNET_ENABLE_ASSERTS
#endif


#ifdef RNET_ENABLE_ASSERTS
	#define ASSERT(validation) assert((validation))
	#define RNET_ASSERT(validation, ...) if(!(validation)) {RNET_LOG_ERROR(__VA_ARGS__);__debugbreak();}
#else //ifdef RNET_ENABLE_ASSERTS
	#define ASSERT(validation)
	#define RNET_ASSERT(validation, ...)
#endif //ifdef RNET_ENABLE_ASSERTS else