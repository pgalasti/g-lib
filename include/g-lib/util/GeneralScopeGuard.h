#ifndef GLIB_GENERAL_SCOPE_GUARD_H
#define GLIB_GENERAL_SCOPE_GUARD_H

namespace GLib::Util {

  // Usage:
  // {
  //   GeneralScopeGuard<decltype(someFuncOrLambda)>(someFuncOrLambda) gsg;
  //   ...
  // } // RAII to invoke handled function

  template<typename F>
  class GeneralScopeGuard {
    F m_Func;

  public:

    explicit GeneralScopeGuard(F func) : m_Func {func} {}
    ~GeneralScopeGuard() {
      func();
    }
  };

  template <typename F> using GSG = GeneralScopeGuard<F>;

} // namespace GLib::Util

#endif // GLIB_GENERAL_SCOPE_GUARD_H
