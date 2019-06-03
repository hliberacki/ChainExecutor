#ifndef CHAINEXECUTOR_INCLUDE_CHAINEXECUTOR_HPP
#define CHAINEXECUTOR_INCLUDE_CHAINEXECUTOR_HPP

namespace executor
{
	template<class T>
	using getSize = typename std::tuple_size<T>::value;

	template<std::size_t Iter, std::size_t Last, template<class...> class Container, typename... Ts>
	typename std::enable_if<Iter == Last, void>::type universal_for_each(Container<Ts...>& container)
	{ 
	}

	template<std::size_t Iter, std::size_t Last, template<class...> class Container = std::tuple,  typename... Ts>
	typename std::enable_if<Iter < Last, void>::type universal_for_each(Container<Ts...>& container)
	{
    std::get<Iter>(container)();
    universal_for_each<Iter+1, Last, Container, Ts...>(container);
	}

	template<std::size_t Iter = 0, template<class...> class Container, typename... Ts>
	void universal_for_each(Container<Ts...>& container)
	{
  	universal_for_each<Iter, sizeof...(Ts), Container, Ts...>(container);
	}

	template<typename Iter> 
	void universal_for_each(Iter first, Iter last)
	{
    	for(; first != last; ++first)
    	{
      	 (*first)();
    	}
	}


	template<typename Filter, typename Input, typename Output>
	struct Node
	{
    Node(Filter f, Input i) : impl{f}, in{i}, out{} {};
    Node(Filter f) : impl{f}, in{}, out{} {};
    
    void operator()()
    {
       out = std::apply(impl, in);
    }
    
    void operator()(Input inputData)
    {
        in = inputData;
        this();
    }
    
    template<size_t I, typename T>
    typename std::enable_if< (I <= std::tuple_size<Input>::value && I > 0), void>::type 
    setInputVectorValue(T el)
    {
        static_assert(std::is_same_v<T, std::tuple_element<I, Input>::type>, 
						          "T is not the same type as Input[I] element");
    }
    
    //Validation if I is out of range
    template<size_t I, typename T>
    typename std::enable_if< (I > std::tuple_size<Input>::value || I < 0), void>::type 
    setInputVectorValue(T el)
    {
        static_assert( ( (I > std::tuple_size<Input>::value) || (I < 0) ), 
						          "I is in range of [0:Input::size]");
    }
    
    Filter impl;
    Input in;
    Output out;
	};


	template<class... Nodes>
	struct ChainProcessor
	{
 		ChainProcessor(Nodes&... nodes) : m_Nodes(std::tie(nodes...)){};
 		std::tuple<Nodes&...> m_Nodes;
 
 		auto process()
 		{
    	universal_for_each(m_Nodes);
    	return getResult<std::tuple_size<decltype(m_Nodes)>::value - 1>();
 		}
  
 		template<size_t I = 0, typename... Args>
 		auto process(Args... args)
 		{
    	setInput<I>(args...);
    	return process();
 		}
 
 		template<class T>
 		auto getResult(T t) { return std::get<decltype(t)>(m_Nodes).out; }
 
 		template<size_t I>
 		auto getResult() { return std::get<I>(m_Nodes).out; }
  
 		template<size_t I = 0, typename... Args>
 		void setInput(Args... args) { std::get<I>(m_Nodes).in = std::make_tuple(args...); }
};



}

#endif //CHAINEXECUTOR_INCLUDE_CHAINEXECUTOR_HPP
