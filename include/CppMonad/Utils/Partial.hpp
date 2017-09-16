#pragma once
#include <tuple>

namespace CppMonad {
	template <class Func, class... Args> class PartialHolder;
	template <class T> PartialHolder<std::remove_reference_t<T>> Partial(T&& func);
	
	template <class Func, class... Args>
	class PartialHolder {
		Func func_;
		std::tuple<Args...> args_;
		
		template <std::size_t... Index, class... CallArgs>
		auto invoke(std::index_sequence<Index...>, CallArgs&&... args) const& {
			return func_(std::get<Index>(args_)..., std::forward<CallArgs>(args)...);
		}
		
		template <std::size_t... Index, class... CallArgs>
		auto invoke(std::index_sequence<Index...>, CallArgs&&... args) && {
			return func_(std::move(std::get<Index>(args_))..., std::forward<CallArgs>(args)...);
		}
		
		template <std::size_t... Index, class... CallArgs>
		auto next(std::index_sequence<Index...>, CallArgs&&... args) const& {
			return PartialHolder<Func, Args..., std::remove_reference_t<CallArgs>...>(
				func_, std::get<Index>(args_)..., std::forward<CallArgs>(args)...);
		}
		
		template <std::size_t... Index, class... CallArgs>
		auto next(std::index_sequence<Index...>, CallArgs&&... args) && {
			return PartialHolder<Func, Args..., std::remove_reference_t<CallArgs>...>(
				func_, std::move(std::get<Index>(args_))..., std::forward<CallArgs>(args)...);
		}
		
		template <class ForwardFunc, class... ForwardArgs>
		PartialHolder(ForwardFunc&& func, ForwardArgs&&... args) :
			func_(std::forward<ForwardFunc>(func)),
			args_(std::forward<ForwardArgs>(args)...) { }
		
		template <class, class...> friend class PartialHolder;
		template <class T> friend PartialHolder<std::remove_reference_t<T>> Partial(T&&);
		
	public:
		PartialHolder(PartialHolder&) = default;
		PartialHolder(PartialHolder&&) = default;
		PartialHolder(const PartialHolder&) = default;
		
		template <class... CallArgs>
		auto operator()(CallArgs&&... args) const& {
			if constexpr (std::is_invocable_v<
				Func, Args..., std::remove_reference_t<CallArgs>...>) {
				return invoke(
					std::make_index_sequence<sizeof...(Args)>(),
					std::forward<CallArgs>(args)...);
			} else {
				return next(
					std::make_index_sequence<sizeof...(Args)>(),
					std::forward<CallArgs>(args)...);
			}
		}
		
		template <class... CallArgs>
		auto operator()(CallArgs&&... args) && {
			if constexpr (std::is_invocable_v<
				Func, Args..., std::remove_reference_t<CallArgs>...>) {
				return std::move(*this).invoke(
					std::make_index_sequence<sizeof...(Args)>(),
					std::forward<CallArgs>(args)...);
			} else {
				return std::move(*this).next(
					std::make_index_sequence<sizeof...(Args)>(),
					std::forward<CallArgs>(args)...);
			}
		}
	};
	
	template <class Func>
	PartialHolder<std::remove_reference_t<Func>> Partial(Func&& func) {
		return PartialHolder<std::remove_reference_t<Func>>(std::forward<Func>(func));
	}
}
