#include "stl.hpp"

#include <vector>

void stl::init()
{

	// 初始化
	std::vector<int> a(1);
	// 初始化
	std::vector b{ 1 };
	//
	const std::vector c(10, 5);

	std::vector d(c);


	// 编写一个支持并行调用的远程过程调用 rpc
	// tc 用于支持线程本地的内存，从而加速调用，避免过程中发生竞争
	// 4KB 的 tc 空间，使用1个64位的原子，64 bit 标记64个原子状态代表区域是否被使用， 仅在首次使用时被绑定到线程
	// 当信号被触发时，则处理
	// tc 的数量与通知机制，使用 8 个信号量等待线程如果完全触发则增加1个
	// 使用进程间的原子变量


	//  4KB 以上数据直接使用托管共享内存段
}
