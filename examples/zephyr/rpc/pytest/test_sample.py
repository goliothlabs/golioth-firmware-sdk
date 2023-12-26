from golioth import RPCStatusCode
import logging
import pytest
import time
import yaml

LOGGER = logging.getLogger(__name__)

pytestmark = pytest.mark.anyio

async def test_logging(shell, device, credentials_file):
    # Read credentials

    with open(credentials_file, 'r') as f:
        credentials = yaml.safe_load(f)

    time.sleep(2)

    # Set credentials

    for setting in credentials['settings']:
        shell.exec_command(f"settings set {setting} \"{credentials['settings'][setting]}\"")

    shell._device.clear_buffer()
    shell._device.write('kernel reboot cold\n\n'.encode())

    # Wait for device to reboot and connect

    shell._device.readlines_until(regex=".*rpc_sample: Golioth client connected", timeout=90.0)

    # Test successful RPC

    result = await device.rpc.call("multiply", [ 7, 6 ])
    LOGGER.info("### Received: {0} Expected: {1}".format(result['value'],  42))
    assert result['value'] == 42, "Didn't receive correct value"

    # Test successful RPC using float

    result = await device.rpc.call("multiply", [ 11.4, 93.81 ])
    LOGGER.info("### Received: {0} Expected: {1}".format(result['value'],  1069.434))
    assert result['value'] == 1069.434, "Didn't receive correct float"

    # Test invalid argument RPC

    error_code = None
    try:
        result = await device.rpc.call("multiply", [ 6, 'J' ])
    except Exception as e:
        error_code = e
        pass
    LOGGER.info("### Received: {0} Expected: {1}".format(error_code.status_code,  RPCStatusCode.INVALID_ARGUMENT))
    assert error_code.status_code == RPCStatusCode.INVALID_ARGUMENT, "Didn't receive correct error code"
