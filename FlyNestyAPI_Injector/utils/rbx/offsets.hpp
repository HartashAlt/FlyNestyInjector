#include <cstdint>

using namespace std;

namespace rbx {
	namespace offsets {
		namespace script {
			constexpr uint64_t msbytecode = 0x0;
			constexpr uint64_t lsbytecode = 0x0;

			constexpr uint64_t moduleflags = 0x0;
			constexpr uint64_t iscore = 0x0;
		}

		namespace datamodel {
			constexpr uint64_t dm1 = 0x0;
			constexpr uint64_t dm2 = 0x0;
			constexpr uint64_t placeid = 0x0;
			constexpr uint64_t gameloaded = 0x0;
		}

		namespace visualengine {
			constexpr uint64_t engine = 0x0;
			constexpr uint64_t viewmatrix = 0x0;
			constexpr uint64_t dimensions = 0x0;
		}

		namespace instance {
			constexpr uint64_t childsize = 0x0;
			constexpr uint64_t children = 0x0;
			constexpr uint64_t parent = 0x0;
			constexpr uint64_t name = 0x0;
			constexpr uint64_t cname = 0x0;
			constexpr uint64_t cdescriptor = 0x0;
			constexpr uint64_t primitive = 0x0;

			namespace basepart {
				constexpr uint64_t position = 0x0;
				constexpr uint64_t cframe = 0x0;
				constexpr uint64_t size = 0x0;
				constexpr uint64_t velocity = 0x0;
				constexpr uint64_t rotvelocity = 0x0;
				constexpr uint64_t anchored = 0x0;
				constexpr uint64_t cancollide = 0x0;
			}

			namespace instancevalue {
				constexpr uint64_t value = 0x0;
			}
		}

		namespace player {
			constexpr uint64_t localplayer = 0x0;
			constexpr uint64_t character = 0x0;
			constexpr uint64_t userid = 0x0;
			constexpr uint64_t displayname = 0x0;
			constexpr uint64_t team = 0x0;
			constexpr uint64_t teamcolor = 0x0;
			constexpr uint64_t cameramaxzoom = 0x0;
			constexpr uint64_t cameraminzoom = 0x0;

			namespace characterinstance {
				namespace humanoid {
					constexpr uint64_t rigtype = 0x0;
					constexpr uint64_t health = 0x0;
					constexpr uint64_t maxhealt = 0x0;
					constexpr uint64_t walkspeed = 0x0;
					constexpr uint64_t walkspeedcheck = 0x0;
					constexpr uint64_t jumppower = 0x0;
					constexpr uint64_t hipheight = 0x0;
				}
			}
		}
	}
}