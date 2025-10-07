set(PROXY_ROOT  ${CMAKE_CURRENT_SOURCE_DIR}/Headers)

# очистити стару папку з проксі, якщо вона існує
if(EXISTS "${PROXY_ROOT}")
  file(REMOVE_RECURSE "${PROXY_ROOT}")
endif()

# створити заново корінь
file(MAKE_DIRECTORY "${PROXY_ROOT}")

set(PROOT_ ${PROXY_ROOT}/StockSDK)

set_property(GLOBAL PROPERTY SDK_PROXY_FILES "")

# function(make_proxy _proxy_path _real_header)
#   get_filename_component(_dir "${_proxy_path}" DIRECTORY)
#   file(MAKE_DIRECTORY "${_dir}")
#   file(WRITE "${_proxy_path}"
# "#include <${_real_header}> // IWYU pragma: export")
# endfunction()

function(make_proxy _proxy_path _real_header)
  get_filename_component(_dir  "${_proxy_path}" DIRECTORY)
  get_filename_component(_name "${_proxy_path}" NAME)
  file(MAKE_DIRECTORY "${_dir}")

  # 1) без розширення (як у Qt)
  set(_proxy_null "${_dir}/${_name}")
  file(WRITE "${_proxy_null}"
"#include <${_real_header}> // IWYU pragma: export")

  # 2) .h-двійник — корисно для IDE, можна прибрати якщо не хочеш
  set(_proxy_h "${_dir}/${_name}.h")
  file(WRITE "${_proxy_h}"
"#include <${_real_header}> // IWYU pragma: export")

  # 3) додати обидва у глобальний список
  # set_property(GLOBAL APPEND PROPERTY SDK_PROXY_FILES "${_proxy_path}")
  set_property(GLOBAL APPEND PROPERTY SDK_PROXY_FILES "${_proxy_h}")
  set_property(GLOBAL APPEND PROPERTY SDK_PROXY_FILES "${_proxy_null}")
endfunction()

make_proxy("${PROOT_}/Loader" "loader.h")
make_proxy("${PROOT_}/SDK" "sdk.h")
make_proxy("${PROOT_}/SDKDef" "sdk_def.h")

make_proxy("${PROOT_}/api/ai/OpenAI" "api/ai/openai.h")
# include/api/broker/ibkr.h
# include/api/broker/tradernet.h
make_proxy("${PROOT_}/api/connectors/EndPoints" "api/connectors/endpoints.h")
make_proxy("${PROOT_}/api/eod/AlphaVantage" "api/eod/alphavantage.h")
make_proxy("${PROOT_}/api/eod/Eod" "api/eod/eod.h")
make_proxy("${PROOT_}/api/eod/Figi" "api/eod/figi.h")
make_proxy("${PROOT_}/api/eod/FinnHub" "api/eod/finnhub.h")
make_proxy("${PROOT_}/api/eod/MarketStack" "api/eod/marketstack.h")
make_proxy("${PROOT_}/api/eod/TwelveData" "api/eod/twelvedata.h")
make_proxy("${PROOT_}/api/eod/Yahoo" "api/eod/yahoo.h")
make_proxy("${PROOT_}/api/transport/FileFetch" "api/transport/filefetch.h")
make_proxy("${PROOT_}/api/transport/Provider" "api/transport/provider.h")
make_proxy("${PROOT_}/api/transport/Call" "api/transport/call.h")
make_proxy("${PROOT_}/api/transport/Syncer" "api/transport/syncer.h")

make_proxy("${PROOT_}/common/DTime" "common/dtime.h")
make_proxy("${PROOT_}/common/Json" "common/json.h")
make_proxy("${PROOT_}/common/Define" "common/define.h")
make_proxy("${PROOT_}/common/Features" "common/features.h")
make_proxy("${PROOT_}/common/ResDir" "common/res_dir.h")
make_proxy("${PROOT_}/common/Trackable" "common/trackable.h")

make_proxy("${PROOT_}/Enum" "core/enum.h")
make_proxy("${PROOT_}/Isin" "core/isin.h")
make_proxy("${PROOT_}/Symbol" "core/symbol.h")
make_proxy("${PROOT_}/forex/Pair" "core/forex/pair.h")

make_proxy("${PROOT_}/security/Data" "core/security/data.h")
make_proxy("${PROOT_}/security/Instrumnet" "core/security/instrument.h")
make_proxy("${PROOT_}/security/Legal" "core/security/legal.h")
make_proxy("${PROOT_}/security/Meta" "core/security/meta.h")

make_proxy("${PROOT_}/security/finance/Capital" "core/security/finance/capital.h")
make_proxy("${PROOT_}/security/finance/Estimate" "core/security/finance/estimate.h")
make_proxy("${PROOT_}/security/finance/Finance" "core/security/finance/finance.h")
make_proxy("${PROOT_}/security/finance/Fundamental" "core/security/finance/fundamental.h")

make_proxy("${PROOT_}/security/listing/CorporateAction" "core/security/listing/corporate_action.h")
make_proxy("${PROOT_}/security/listing/Dividend" "core/security/listing/dividend.h")
make_proxy("${PROOT_}/security/listing/Quotes" "core/security/listing/quotes.h")
make_proxy("${PROOT_}/security/listing/ShortInterest" "core/security/listing/short_interest.h")
make_proxy("${PROOT_}/security/listing/Ticker" "core/security/listing/ticker.h")
make_proxy("${PROOT_}/security/listing/Valuation" "core/security/listing/valuation.h")

make_proxy("${PROOT_}/security/quarter/Balance" "core/security/quarter/balance.h")
make_proxy("${PROOT_}/security/quarter/CashFlow" "core/security/quarter/cash_flow.h")
make_proxy("${PROOT_}/security/quarter/Earnings" "core/security/quarter/earnings.h")
make_proxy("${PROOT_}/security/quarter/Incomes" "core/security/quarter/incomes.h")
make_proxy("${PROOT_}/security/quarter/QuarterData" "core/security/quarter/quarter_data.h")
make_proxy("${PROOT_}/security/quarter/Trend" "core/security/quarter/trend.h")

make_proxy("${PROOT_}/service/Finder" "service/finder.h")
make_proxy("${PROOT_}/service/Forex" "service/forex.h")
make_proxy("${PROOT_}/service/Market" "service/market.h")
make_proxy("${PROOT_}/service/Roster" "service/roster.h")

make_proxy("${PROOT_}/user/account/Account" "user/account/account.h")
make_proxy("${PROOT_}/user/account/Profile" "user/account/profile.h")
make_proxy("${PROOT_}/user/account/Session" "user/account/session.h")
make_proxy("${PROOT_}//user/account/Identity" "user/account/identity.h")

make_proxy("${PROOT_}/user/ledger/Ledger" "user/ledger/ledger.h")
make_proxy("${PROOT_}/user/ledger/Pending" "user/ledger/pending.h")
make_proxy("${PROOT_}/user/ledger/Portfolio" "user/ledger/portfolio.h")
make_proxy("${PROOT_}/user/ledger/Screener" "user/ledger/screener.h")
make_proxy("${PROOT_}/user/ledger/Targets" "user/ledger/targets.h")
make_proxy("${PROOT_}/user/ledger/WatchList" "user/ledger/watchlist.h")

make_proxy("${PROOT_}/user/stock/Stock" "user/stock/stock.h")
make_proxy("${PROOT_}/user/stock/Transaction" "user/stock/transaction.h")

target_include_directories(sdk PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${PROXY_ROOT}>
  $<INSTALL_INTERFACE:include>
)

# дістати список усіх згенерованих проксі
get_property(_SDK_PROXIES GLOBAL PROPERTY SDK_PROXY_FILES)

# Якщо у тебе CMake 3.23+ — ідеально: FILE_SET HEADERS (як це робить Qt)
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.23")
  target_sources(sdk
    PUBLIC
      FILE_SET sdk_public_headers TYPE HEADERS
      BASE_DIRS ${PROXY_ROOT}
      FILES ${_SDK_PROXIES}
  )
else()
  # fallback: просто додай у target_sources і познач як HEADER_FILE_ONLY
  target_sources(sdk PUBLIC ${_SDK_PROXIES})
  set_source_files_properties(${_SDK_PROXIES} PROPERTIES HEADER_FILE_ONLY TRUE)
endif()

install(DIRECTORY "${PROXY_ROOT}/" DESTINATION include)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/" DESTINATION include)
