import torch
import yaml
from weaver.nn.model.ParticleTransformer import ParticleTransformer
from weaver.utils.logger import _logger


def load_config_from_yaml(config_path):
    with open(config_path, 'r') as file:
        return yaml.safe_load(file)

class ParticleTransformerWrapper(torch.nn.Module):
    def __init__(self, **kwargs) -> None:
        super().__init__()
        self.mod = ParticleTransformer(**kwargs)

    @torch.jit.ignore
    def no_weight_decay(self):
        return {'mod.cls_token', }

    def forward(self, points, features, lorentz_vectors, mask):
        return self.mod(features, v=lorentz_vectors, mask=mask)

def get_model(data_config, **kwargs):
    cfg = dict(
        input_dim=len(data_config['inputs']['pf_features']),
        num_classes=len(data_config['labels']),
        pair_input_dim=4,
        use_pre_activation_pair=False,
        embed_dims=[128, 512, 128],
        pair_embed_dims=[64, 64, 64],
        num_heads=8,
        num_layers=8,
        num_cls_layers=2,
        block_params=None,
        cls_block_params={'dropout': 0, 'attn_dropout': 0, 'activation_dropout': 0},
        fc_params=[],
        activation='gelu',
        trim=True,
        for_inference=False,
    )
    cfg.update(**kwargs)
    _logger.info('Model config: %s' % str(cfg))

    model = ParticleTransformerWrapper(**cfg)
    return model
config_path = '/home/liang/Workingspace/PartTransformer/particle_transformer-main/data/JetClass/JetClass_full.yaml'
data_config = load_config_from_yaml(config_path)
model = get_model(data_config)
print(model)


