import synodic.cppaste;

struct ConsumerInterface;
struct ConsumerImplementation;

using ConsumerInterfaceBase = cppaste::InterfaceBase<
	ConsumerInterface,
	ConsumerImplementation,
	cppaste::InterfaceMode::STATIC
>;

static_assert(ConsumerInterfaceBase::allows_static);

int main()
{
	return 0;
}
