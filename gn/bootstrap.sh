# This script must be sourced, not executed.
#
# Create a new environment:
#
#   source bootstrap.sh
#
# Activate an existing environment:
#
#   source activate.sh

_bootstrap_abspath () {
  $(command -v python3 || command -v python2 || command -v python) -c "import os.path; print(os.path.abspath('$@'))"
}

# Shell: bash.
if test -n "$BASH"; then
  _BOOTSTRAP_PATH="$(_bootstrap_abspath "$BASH_SOURCE")"
# Shell: zsh.
elif test -n "$ZSH_NAME"; then
  _BOOTSTRAP_PATH="$(_bootstrap_abspath "${(%):-%N}")"
# Shell: dash.
elif test ${0##*/} = dash; then
  _BOOTSTRAP_PATH="$(_bootstrap_abspath \
    "$(lsof -p $$ -Fn0 | tail -1 | sed 's#^[^/]*##;')")"
# If everything else fails, try $0. It could work.
else
  _BOOTSTRAP_PATH="$(_bootstrap_abspath "$0")"
fi

# Check if this file is being executed or sourced.
_pw_sourced=0
if [ -n "$SWARMING_BOT_ID" ]; then
  # If set we're running on swarming and don't need this check.
  _pw_sourced=1
elif [ -n "$ZSH_EVAL_CONTEXT" ]; then
  case $ZSH_EVAL_CONTEXT in *:file) _pw_sourced=1;; esac
elif [ -n "$KSH_VERSION" ]; then
  [ "$(cd $(dirname -- $0) && pwd -P)/$(basename -- $0)" != \
    "$(cd $(dirname -- ${.sh.file}) && pwd -P)/$(basename -- ${.sh.file})" ] \
    && _pw_sourced=1
elif [ -n "$BASH_VERSION" ]; then
  (return 0 2>/dev/null) && _pw_sourced=1
else  # All other shells: examine $0 for known shell binary filenames
  # Detects `sh` and `dash`; add additional shell filenames as needed.
  case ${0##*/} in sh|dash) _pw_sourced=1;; esac
fi

# Downstream projects need to set something other than PW_ROOT here, like
# YOUR_PROJECT_ROOT. Please also set PW_PROJECT_ROOT and PW_ROOT before
# invoking pw_bootstrap or pw_activate.
######### BEGIN PROJECT-SPECIFIC CODE #########
PROJECT_ROOT="$(_bootstrap_abspath "$(dirname "$_BOOTSTRAP_PATH")")"
export PROJECT_ROOT
PW_PROJECT_ROOT="$PROJECT_ROOT"

# Set your project's banner and color.
export PW_BRANDING_BANNER="$PROJECT_ROOT/banner.txt"
export PW_BRANDING_BANNER_COLOR=magenta
project_banner() {
  cat "$PW_BRANDING_BANNER"
  echo
}
PW_BANNER_FUNC="project_banner"

# Do full clones of deps instead of struggling with submodules.
PW_ROOT="$PROJECT_ROOT/third_party/pigweed"
if [ ! -d $PW_ROOT ]
then
  PW_URL="https://github.com/google/pigweed"
  git clone --recursive $PW_URL $PW_ROOT
  cd $PW_ROOT
  PW_COMMIT="dea2ecf94d39c9c6f97140190bc6a796d5205b96"  # 20240423
  git checkout $PW_COMMIT
  cd -
fi
FREERTOS_ROOT="$PW_PROJECT_ROOT/third_party/freertos"
if [ ! -d $FREERTOS_ROOT ]
then
  FREERTOS_URL="https://github.com/FreeRTOS/FreeRTOS-Kernel"
  FREERTOS_COMMIT="9c048e0c71ee43630394981a86f5265bc57131e4"  # 20201215
  git clone --recursive $FREERTOS_URL $FREERTOS_ROOT
  cd $FREERTOS_ROOT
  git checkout $FREERTOS_COMMIT
  cd -
fi
PICO_ROOT="$PW_PROJECT_ROOT/third_party/pico-sdk"
if [ ! -d $PICO_ROOT ]
then
  PICO_URL="https://github.com/raspberrypi/pico-sdk"
  PICO_COMMIT="6a7db34ff63345a7badec79ebea3aaef1712f374"  # 20230613
  git clone --recursive $PICO_URL $PICO_ROOT
  cd $PICO_ROOT
  git checkout $PICO_COMMIT
  cd -
fi
########## END PROJECT-SPECIFIC CODE ##########
export PW_BANNER_FUNC
export PW_PROJECT_ROOT
export PW_ROOT

_util_sh="$PW_ROOT/pw_env_setup/util.sh"

# Double-check that the Pigweed submodule has been checked out.
if [ ! -e "$_util_sh" ]; then
  echo "Error: $_util_sh not found."
  echo "Did you forget to initialize the git submodules?"
  echo "To setup the git submodules run:"
  echo "  git submodule update --init"
  return
fi

. "$_util_sh"

pw_deactivate
pw_eval_sourced "$_pw_sourced"
pw_check_root "$PW_ROOT"
_PW_ACTUAL_ENVIRONMENT_ROOT="$(pw_get_env_root)"
export _PW_ACTUAL_ENVIRONMENT_ROOT
SETUP_SH="$_PW_ACTUAL_ENVIRONMENT_ROOT/activate.sh"

# Run full bootstrap when invoked as bootstrap, or env file is missing/empty.
if [ "$(basename "$_BOOTSTRAP_PATH")" = "bootstrap.sh" ] || \
  [ ! -f "$SETUP_SH" ] || \
  [ ! -s "$SETUP_SH" ]; then
# This is where pw_bootstrap is called. Most small projects will include
# --use-pigweed-defaults.
######### BEGIN PROJECT-SPECIFIC CODE #########
  pw_bootstrap --shell-file "$SETUP_SH" --install-dir "$_PW_ACTUAL_ENVIRONMENT_ROOT" --config-file "$PW_PROJECT_ROOT/pigweed.json"
########## END PROJECT-SPECIFIC CODE ##########
  pw_finalize bootstrap "$SETUP_SH"
else
  pw_activate
  pw_finalize activate "$SETUP_SH"
fi

if [ "$_PW_ENV_SETUP_STATUS" -eq 0 ]; then
# This is where any additional checks about the environment should go.
######### BEGIN PROJECT-SPECIFIC CODE #########
  echo -n
########## END PROJECT-SPECIFIC CODE ##########
fi

unset _pw_sourced
unset _BOOTSTRAP_PATH
unset SETUP_SH
unset _bootstrap_abspath
unset _util_sh

pw_cleanup
