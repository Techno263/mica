namespace mica {

template<typename Sig, Sig* Func>
constexpr auto resolve() -> Sig*
{
    return Func;
}

} // namespace mica
