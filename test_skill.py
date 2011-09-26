#import unittest
import skill

class TestSkillCase():
    def test_create_skill(self):
        skill_ = skill.Skill('Test Skill', '10', skill.ERA_IS1920)
        assert 'Test Skill' == skill_.name
        assert '10' == skill_.base
        assert skill.SKILL == skill_.type
        assert skill.ERA_IS1920 == skill_.EraMask
        
    def test_create_empty_skill(self):
        skill_ = skill.Skill()
        assert '' == skill_.name
        assert '' == skill_.base
        assert skill.NO_TYPE == skill_.type
        assert skill.ERA_ISNONE == skill_.EraMask
        
    def test_is_era_pass(self):
        skill_ = skill.Skill('Test Skill', '10', skill.ERA_IS1920)
        assert skill_.IsEra(skill.ERA_IS1920)
    
    def test_is_era_pass_all_era(self):
        skill_ = skill.Skill('Test Skill', '10', skill.ERA_ISALL)
        assert skill_.IsEra(skill.ERA_IS1890)
        assert skill_.IsEra(skill.ERA_IS1920)
        assert skill_.IsEra(skill.ERA_IS1990)
        
    def test_is_era_fail(self):
        skill_ = skill.Skill('Test Skill', '10', skill.ERA_IS1920)
        assert not skill_.IsEra(skill.ERA_IS1890)
        
    def test_get_base_string_simple(self):
        skill_ = skill.Skill('Test Skill', '10', skill.ERA_IS1920)
        assert skill_.GetBaseString(False, True) == '10'
        
    def test_get_base_string_empty(self):
        skill_ = skill.Skill('Test Skill', '0', skill.ERA_IS1920)
        assert skill_.GetBaseString(False, True) == '00'
        
    def test_get_base_string_empty_double_zero(self):
        skill_ = skill.Skill('Test Skill', '00', skill.ERA_IS1920)
        assert skill_.GetBaseString(False, True) == '00'
    
    def test_get_base_string_empty_base01(self):
        skill_ = skill.Skill('Test Skill', '0', skill.ERA_IS1920)
        assert skill_.GetBaseString(True, True) == '01'
        
    def test_get_base_string_single_digit(self):
        skill_ = skill.Skill('Test Skill', '5', skill.ERA_IS1920)
        assert skill_.GetBaseString(False, True) == '05'